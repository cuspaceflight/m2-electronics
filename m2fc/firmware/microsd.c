#include <stdbool.h>
#include "microsd.h"
#include "hal.h"
#include "ff.h"
#include "chprintf.h"

/* ------------------------------------------------------------------------- */

static bool microsd_card_init(FATFS* fs);
static void microsd_card_try_init(FATFS* fs);
static void microsd_card_deinit(void);

/* ------------------------------------------------------------------------- */

static bool microsd_card_init(FATFS* fs)
{
    FRESULT err;

    /* Initialise the SDC interface */
    sdcStart(&SDCD1, NULL);

    /* Attempt to connect to the SD card */
    int i = sdcConnect(&SDCD1);
    if (i) {
        return false;
    }

    /* Attempt to mount the filesystem */
    err = f_mount(0, fs);
    return err == FR_OK;
}

static void microsd_card_try_init(FATFS* fs)
{
    while(!microsd_card_init(fs)) {
        microsd_card_deinit();
        chThdSleepMilliseconds(200);
    }
}

static void microsd_card_deinit()
{
    /* Unmount FS */
    f_mount(0, NULL);

    /* Disconnect from card */
    sdcDisconnect(&SDCD1);

    /* Disable SDC peripheral */
    sdcStop(&SDCD1);
}

/* ------------------------------------------------------------------------- */

/* open file in <path> to <fp>.
 * mounts the file system and everything; blocking (see try_init function).
 * NOTE!!! Assumes only one file opened at a time - might want to check if the
 * sd card/file system is already connected/mounted when opening (is this even
 * possible though).
 */
SDRESULT microsd_open_file(SDFILE* fp, const char* path, SDMODE mode,
    SDFS* sd)
{
    SDRESULT err;
    microsd_card_try_init(sd);
    err = f_open(fp, path, mode);
    return err;
}

/* Open/create file using incremental naming scheme that follows the format
 * <filename>_<5-digit number>.<extension>.
 * E.g. if log_00001.bin exists, try log_00002.bin until we find one that
 * doesn't already exist or we reach the limit of 99999.
 */
SDRESULT microsd_open_file_inc(FIL* fp, const char* path, const char* ext,
    SDFS* sd)
{
    SDRESULT err;
    SDMODE mode = FA_WRITE | FA_CREATE_NEW;
    uint32_t file_idx = 0;
    char fname[25];

    microsd_card_try_init(sd);

    while (true) {
        // try to open file with number file_idx
        file_idx++;
        chsnprintf(fname, 25, "%s_%05d.%s", path, file_idx, ext);
        err = f_open(fp, fname, mode);
        if (err == FR_EXIST) continue;
        else return err;
    }
}

/* Close file in <fp>.
 * Unmounts the file system and disconnects from the SD card as well.
 */
SDRESULT microsd_close_file(SDFILE* fp)
{
    SDRESULT err;
    err = f_close(fp);
    microsd_card_deinit();
    return err;
}

/* Write <btw> bytes from <buf> to <fp>.
 * Number of bytes written is currently not used for anything ...
 * If bytes_written < btw aftewards, disk is full.
 */
SDRESULT microsd_write(SDFILE* fp, const char* buf, unsigned int btw)
{
    SDRESULT err;
    unsigned int bytes_written;

    palSetPad(GPIOA, GPIOA_LED_SDCARD);
    err = f_write(fp, (void*) buf, btw, &bytes_written);
    f_sync(fp);
    palClearPad(GPIOA, GPIOA_LED_SDCARD);

    return err;
}

/* Read <btr> bytes from <fp> to <buf>.
 * Number of bytes read is currently not used for anything ...
 * If bytes_read < btr afterwards, reached end of file.
 */
SDRESULT microsd_read(SDFILE* fp, char* buf, unsigned int btr)
{
    SDRESULT err;
    unsigned int bytes_read;
    err = f_lseek(fp, 0);
    if (err == FR_OK) {
        err = f_read(fp, (void*)buf, btr, &bytes_read);
    }
    return err;
}

/* Same as microsd_read, except reads only up to newline.
 */
SDRESULT microsd_gets(SDFILE* fp, char* buf, int size)
{
    TCHAR* res = f_gets(buf, size, fp);
    // if res is null then either an error occurred (check with f_error(fp))
    // or it reached end of file (check with f_eof(fp))
    return res == NULL ? FR_INT_ERR : FR_OK;
}
