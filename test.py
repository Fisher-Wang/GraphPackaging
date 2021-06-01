import os
import subprocess

ids = ["8_10", "16_6", "24_4", "32_3"]
result_path = 'result.txt'
edge_paths = ["data/" + "Out_OutGraph_Basketball_480_Slice16_Gop" + i + ".log" for i in ids]
weight_paths = ["data/" + "Out_SliceSize_Basketball_480_Slice16_Gop" + i + ".log" for i in ids]

exe_paths = [
    # 'try_basic',
    'try_dfs_naive',
]
checker_path = 'checker'

for exe_path in exe_paths:
    print('Testing {}'.format(exe_path))
    for i in range(4):
        print('Case {}'.format(i))
        result = subprocess.run([exe_path, edge_paths[i], weight_paths[i], result_path], 
                                stdout=subprocess.PIPE, stderr=subprocess.STDOUT, encoding='utf-8')
        print(result.stdout)
        result = subprocess.run([checker_path, edge_paths[i], weight_paths[i], result_path], 
                                stdout=subprocess.PIPE, stderr=subprocess.STDOUT, encoding='utf-8')
        print(result.stdout)