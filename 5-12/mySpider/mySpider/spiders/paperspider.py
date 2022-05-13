import scrapy
import json
from mySpider import items
import re

class PaperspiderSpider(scrapy.Spider):
    name = 'paperspider'
    allowed_domains = ['xs.dailyheadlines.cc']
    start_urls = ['https://xs.dailyheadlines.cc/scholar?hl=zh-CN&as_sdt=0,5&q=Image+restoration+under+the+water&btnG=']

    #重新定义request，使用POST方法
    def start_requests(self):
        formdata={
            "hl":"zh-CN",
            "as_sdt":"0,5",
            "q":"Image+restoration+under+the+water",
            "btnG":""
        }
        for i in range(20):
            next_url="https://xs.dailyheadlines.cc/scholar?start="+str(i*10)+"&q=Image+restoration+under+the+water&hl=zh-CN&as_sdt=0,5"
            yield scrapy.Request(next_url,method="POST")
        # yield scrapy.Request(self.start_urls[0],method="POST")

    def parse(self, response):
        # title_list = response.xpath('//*[@id="gs_res_ccl_mid"]/div/div[2]/h3')
        # # print(len(node_list))
        # for title in title_list:
        #     item=items.MyspiderItem()
        #     item["papertitle"]=title.xpath('string(./a)').extract_first()


        #1.提取数据
        block_list = response.xpath('//*[@id="gs_res_ccl_mid"]/div/div[2]')
        for block in block_list:
            item=items.MyspiderItem()
            item["papertitle"]=block.xpath('string(./h3/a)').extract_first()
            # print(item["papertitle"]) 
            item["paperlink"]=response.urljoin(block.xpath('./h3/a/@href').extract_first())
            # print(item["paperlink"])
            item["paperfrom"]=block.xpath('string(./div[1])').extract_first()
            # print(item["paperfrom"])
            item["abstract"]=block.xpath('string(./div[2])').extract_first()

            yield item
        #2.翻页已经在start中实现
            # url=response.xpath('//*[@id="gs_nm"]/button[2]/@onclick').extract_first()
            # next_url=re.findall('/scholar?(.+?)\'',url)[0]
            # print(response.urljoin(next_url).decode("unicode-escape"))


