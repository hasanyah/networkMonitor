import requests
import time

toggle = True
while True:
    r = requests.get('https://xkcd.com/1906/')
    print('Received 1')
    if toggle:
        r = requests.get('https://png.pngtree.com/element_our/png_detail/20181226/baby-unicorn-lies-in-flowers-png_275609.jpg')
        print('Received 2')
    toggle = not toggle
    time.sleep(1)
