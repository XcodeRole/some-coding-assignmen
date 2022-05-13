# Define here the models for your scraped items
#
# See documentation in:
# https://docs.scrapy.org/en/latest/topics/items.html

import scrapy


class MyspiderItem(scrapy.Item):
    # define the fields for your item here like:
    #论文名称
    papertitle = scrapy.Field()
    #论文链接
    paperlink = scrapy.Field()
    #所在期刊
    paperfrom = scrapy.Field()
    #一作
    authorname = scrapy.Field()
    #单位
    authorcompany = scrapy.Field()
    #时间
    time = scrapy.Field()
    #关键词
    keyword = scrapy.Field()
    #摘要
    abstract = scrapy.Field()


