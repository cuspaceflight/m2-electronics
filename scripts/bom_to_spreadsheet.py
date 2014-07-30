import json
import requests
from lxml import html
import sys

parts = {}

if sys.argv[1] == "-d":
    print("loading parts from json dump")
    parts = json.load(open("dump.json"))
else:
    n = 0
    with open(sys.argv[1]) as f:
        for line in f:
            if line.strip() == "":
                break
            print(n)
            n += 1
            number, qty, comment = line.split(", ")
            comment = comment.strip()
            qty = int(qty)
            url = "http://onecall.farnell.com/_/_/_/dp/{}".format(number)
            req = requests.get(url)
            tree = html.fromstring(req.text)
            try:
                title = tree.xpath('//*[@id="headerContainer"]/h1/text()')[0]
            except:
                title = "ERR"
            try:
                price = tree.xpath(
                    '//*[@class="nowrap mfProductDescriptionAndPrice"]/text()')
                price = price[2].strip()[1:]
            except:
                price = "ERR"
            try:
                stock = tree.xpath('//*[@id="orderDetailsContainer"]'
                                   '/div/div[1]/div[1]/b[1]/text()')
                stock = stock[0]
            except:
                stock = "ERR"
            try:
                cost = float(price) * int(qty)
            except:
                cost = "ERR"
            parts[number] = {"qty": qty, "url": url, "number": number,
                             "comment": comment, "stock": stock,
                             "name": title.strip(), "price": price,
                             "cost": cost}

    with open("dump.json", "w") as f:
        json.dump(parts, f)


with open(sys.argv[1]+"_spread.csv", "w") as f:
    f.write("number,comment,name,stock,price,qty,cost,url\n")
    for number in parts:
        part = parts[number]
        s = '"{}","{}","{}","{}","{}","{}","{}","{}"\n'.format(
            part['number'], part['comment'], part['name'], part['stock'],
            part['price'], part['qty'], part['cost'], part['url'])
        f.write(s)
