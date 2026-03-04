import sys

args = sys.argv
sourse_file_name = args[1]
result_file = open(sourse_file_name.removesuffix('.dmp') + '_result.txt', 'w')
with open(sourse_file_name, 'r') as f:
    for line in f:
        result = line.split('v:')[1].split(')')[0]
        result_file.write(str(int(result, 0))+'\n')