#![allow(dead_code)]

extern crate libc;
pub use self::libc::{c_void, c_int, c_char};

#[repr(C)]
pub struct airspy_device;

#[repr(C)]
#[derive(Debug,Copy,Clone)]
pub enum airspy_error {
    AIRSPY_SUCCESS = 0,
    AIRSPY_TRUE = 1,
    AIRSPY_ERROR_INVALID_PARAM = -2,
    AIRSPY_ERROR_NOT_FOUND = -5,
    AIRSPY_ERROR_BUSY = -6,
    AIRSPY_ERROR_NO_MEM = -11,
    AIRSPY_ERROR_LIBUSB = -1000,
    AIRSPY_ERROR_THREAD = -1001,
    AIRSPY_ERROR_STREAMING_THREAD_ERR = -1002,
    AIRSPY_ERROR_STREAMING_STOPPED = -1003,
    AIRSPY_ERROR_OTHER = -9999
}

#[repr(C)]
pub enum airspy_board_id {
    AIRSPY_BOARD_ID_PROTO_AIRSPY = 0,
    AIRSPY_BOARD_ID_INVALID = 0xFF
}

#[repr(C)]
pub enum airspy_sample_type {
    AIRSPY_SAMPLE_FLOAT32_IQ = 0,
    AIRSPY_SAMPLE_FLOAT32_REAL = 1,
    AIRSPY_SAMPLE_INT16_IQ = 2,
    AIRSPY_SAMPLE_INT16_REAL = 3,
    AIRSPY_SAMPLE_UINT16_REAL = 4,
    AIRSPY_SAMPLE_END = 5
}

#[repr(C)]
pub struct airspy_lib_version_t {
    pub major_version: u32,
    pub minor_version: u32,
    pub revision: u32
}

#[repr(C)]
pub struct airspy_read_partid_serialno_t {
    pub part_id: [u32; 2],
    pub serial_no: [u32; 4]
}

#[repr(C)]
pub struct airspy_transfer_t {
    pub device: *mut airspy_device,
    pub ctx: *mut c_void,
    pub samples: *mut c_void,
    pub sample_count: c_int,
    pub sample_type: airspy_sample_type
}

#[repr(C)]
pub enum receiver_mode_t {
    RECEIVER_MODE_OFF = 0,
    RECEIVER_MODE_RX = 1
}

#[repr(C)]
pub enum airspy_gpio_port_t {
    GPIO_PORT0 = 0,
    GPIO_PORT1 = 1,
    GPIO_PORT2 = 2,
    GPIO_PORT3 = 3,
    GPIO_PORT4 = 4,
    GPIO_PORT5 = 5,
    GPIO_PORT6 = 6,
    GPIO_PORT7 = 7,
}

#[repr(C)]
pub enum airspy_gpio_pin_t {
    GPIO_PIN0 = 0,
    GPIO_PIN1 = 1,
    GPIO_PIN2 = 2,
    GPIO_PIN3 = 3,
    GPIO_PIN4 = 4,
    GPIO_PIN5 = 5,
    GPIO_PIN6 = 6,
    GPIO_PIN7 = 7,
    GPIO_PIN8 = 8,
    GPIO_PIN9 = 9,
    GPIO_PIN10 = 10,
    GPIO_PIN11 = 11,
    GPIO_PIN12 = 12,
    GPIO_PIN13 = 13,
    GPIO_PIN14 = 14,
    GPIO_PIN15 = 15,
    GPIO_PIN16 = 16,
    GPIO_PIN17 = 17,
    GPIO_PIN18 = 18,
    GPIO_PIN19 = 19,
    GPIO_PIN20 = 20,
    GPIO_PIN21 = 21,
    GPIO_PIN22 = 22,
    GPIO_PIN23 = 23,
    GPIO_PIN24 = 24,
    GPIO_PIN25 = 25,
    GPIO_PIN26 = 26,
    GPIO_PIN27 = 27,
    GPIO_PIN28 = 28,
    GPIO_PIN29 = 29,
    GPIO_PIN30 = 30,
    GPIO_PIN31 = 31
}

#[link(name="airspy")]
extern {
    pub fn airspy_lib_version(version: &mut airspy_lib_version_t);
    pub fn airspy_init() -> airspy_error;
    pub fn airspy_exit() -> airspy_error;
    pub fn airspy_open_sn(device: *mut *mut airspy_device, serial_number: u64) -> airspy_error;
    pub fn airspy_open(device: *mut *mut airspy_device) -> airspy_error;
    pub fn airspy_close(device: *mut airspy_device) -> airspy_error;
    pub fn airspy_get_samplerates(device: *mut airspy_device, buffer: *mut u32, len: u32) -> airspy_error;
    pub fn airspy_set_samplerate(device: *mut airspy_device, samplerate: u32) -> airspy_error;
    pub fn airspy_start_rx(device: *mut airspy_device, callback: extern fn(*mut airspy_transfer_t) -> c_int, rx_ctx: *mut c_void) -> airspy_error;
    pub fn airspy_stop_rx(device: *mut airspy_device) -> airspy_error;
    pub fn airspy_is_streaming(device: *mut airspy_device) -> c_int;
    pub fn airspy_si5351c_write(device: *mut airspy_device, register_number: u8, value: u8) -> airspy_error;
    pub fn airspy_si5351c_read(device: *mut airspy_device, register_number: u8, value: *mut u8) -> airspy_error;
    pub fn airspy_r820t_write(device: *mut airspy_device, register_number: u8, value: u8) -> airspy_error;
    pub fn airspy_r820t_read(device: *mut airspy_device, register_number: u8, value: *mut u8) -> airspy_error;
    pub fn airspy_gpio_write(device: *mut airspy_device, port: airspy_gpio_port_t, pin: airspy_gpio_pin_t, value: u8) -> airspy_error;
    pub fn airspy_gpio_read(device: *mut airspy_device, port: airspy_gpio_port_t, pin: airspy_gpio_pin_t, value: *mut u8) -> airspy_error;
    pub fn airspy_gpiodir_write(device: *mut airspy_device, port: airspy_gpio_port_t, pin: airspy_gpio_pin_t, value: u8) -> airspy_error;
    pub fn airspy_gpiodir_read(device: *mut airspy_device, port: airspy_gpio_port_t, pin: airspy_gpio_pin_t, value: *mut u8) -> airspy_error;
    pub fn airspy_spiflash_erase(device: *mut airspy_device) -> airspy_error;
    pub fn airspy_spiflash_write(device: *mut airspy_device, address: u32, length: u16, data: *mut c_char) -> airspy_error;
    pub fn airspy_spiflash_read(device: *mut airspy_device, address: u32, length: u16, data: *mut c_char) -> airspy_error;
    pub fn airspy_board_id_read(device: *mut airspy_device, value: *mut u8) -> airspy_error;
    pub fn airspy_version_string_read(device: *mut airspy_device, version: *mut c_char, length: u8) -> airspy_error;
    pub fn airspy_board_partid_serialno_read(device: *mut airspy_device, read_partid_serialno: *mut airspy_read_partid_serialno_t) -> airspy_error;
    pub fn airspy_set_sample_type(device: *mut airspy_device, sample_type: airspy_sample_type) -> airspy_error;
    pub fn airspy_set_freq(device: *mut airspy_device, freq_hz: u32) -> airspy_error;
    pub fn airspy_set_lna_gain(device: *mut airspy_device, value: u8) -> airspy_error;
    pub fn airspy_set_mixer_gain(device: *mut airspy_device, value: u8) -> airspy_error;
    pub fn airspy_set_vga_gain(device: *mut airspy_device, value: u8) -> airspy_error;
    pub fn airspy_set_lna_agc(device: *mut airspy_device, value: u8) -> airspy_error;
    pub fn airspy_set_mixer_agc(device: *mut airspy_device, value: u8) -> airspy_error;
    pub fn airspy_set_rf_bias(device: *mut airspy_device, value: u8) -> airspy_error;
    pub fn airspy_set_packing(device: *mut airspy_device, value: u8) -> airspy_error;
    pub fn airspy_error_name(errcode: airspy_error) -> *const c_char;
    pub fn airspy_board_id_name(board_id: u8) -> *const c_char;
}
