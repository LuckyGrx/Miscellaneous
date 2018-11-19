# encoding=utf-8 --
import json
import os

def getLastAntiBracketIndex(str):
    """
    获取字符串最后一个反括号的下标
    """
    idx = len(str) - 1
    while idx >= 0:
        if str[idx] == '}':
            return idx
        idx -= 1
    return -1

def bib2json(path):
    """
    bib格式文件转换成json格式文件
    """
    output = path.split('.')[0] + ".json"
    out = open(output, 'w', encoding = 'UTF-8')
    with open(path, 'r', encoding = 'UTF-8') as f:
        input = f.read()
        itemset = input.split('@')[1:]
        for item in itemset:
            js = {}
            title = item.split('{', 1)
            js["type"] = title[0]
            js["id"] = title[1].split(',' ,1)[0]
            for i in title[1].split(',' ,1)[1:][0].split('\n'):
                if len(i) <= 2:
                    continue
                key = i.split("=")[0].strip()
                value = i.split("=")[1]
                if key == "author" or key == "editor":
                    names = value.split('{', 1)[1]
                    names = names[:getLastAntiBracketIndex(names)].split(' and ')
                    newName = ""
                    for k in names:
                        s = k.split(',')
                        if len(s) < 2:
                            temp = k
                        else:
                            temp = s[1].strip() + " " + s[0].strip()
                        if 0 == len(newName):
                            newName += temp
                        else:
                            newName += ", " + temp
                    js["author"] = newName
                else:
                    value = value.split('{', 1)[1]
                    value = value[:getLastAntiBracketIndex(value)]
                    js[key.lower()] = value
            json.dump(js, out, ensure_ascii = False)
            out.write('\n')
    out.close()

if __name__ == "__main__":
    path = os.listdir(os.getcwd())
    for p in path:
        if p.split('.')[-1] == "bib":
            bib2json(p)