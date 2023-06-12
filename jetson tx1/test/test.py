import requests
from bs4 import BeautifulSoup
import urllib

# 爬取的网站和页面
url = 'https://www.example.com/football'

# 获取网页内容
response = requests.get(url)
html = response.content

# 解析网页内容，提取图片链接
soup = BeautifulSoup(html, 'html.parser')
img_tags = soup.find_all('img')
img_urls = [img['src'] for img in img_tags]

# 下载图片并保存到指定的文件夹中
folder_path = 'G:/Vscode/jetson tx1/test'
for url in img_urls:
    filename = url.split('/')[-1]
    urllib.request.urlretrieve(url, folder_path + '/' + filename)