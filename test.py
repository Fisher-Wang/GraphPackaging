import os
import subprocess
import argparse

exe_paths = [
    # 'try_basic',
    # 'try_dfs_naive',
    'try_dfs2',
    # 'try_dfs3',
]

parser = argparse.ArgumentParser()
parser.add_argument('-t', '--test_cases', type=str, default='0123', help='test cases')
parser.add_argument('--result', type=str, default='result.txt', help='result file path')
parser.add_argument('--method', type=str, nargs='+', default=exe_paths)
parser.add_argument('-l', '--loss_prob', type=int, default=20, help='loss probability of the package')

args = parser.parse_args()
testcases = [int(i) for i in list(args.test_cases)]
result_path = args.result
exe_paths = args.method
loss_prob = str(args.loss_prob)


ids = ["8_10", "16_6", "24_4", "32_3"]
edge_paths = ["data/" + "Out_OutGraph_Basketball_480_Slice16_Gop" + i + ".log" for i in ids]
weight_paths = ["data/" + "Out_SliceSize_Basketball_480_Slice16_Gop" + i + ".log" for i in ids]


checker_path = 'checker'

for exe_path in exe_paths:
    print('Testing {}'.format(exe_path))
    for i in testcases:
        print('Case {}'.format(i))
        print('weight_path = {}'.format(weight_paths[i]))
        result = subprocess.run([exe_path, edge_paths[i], weight_paths[i], result_path, loss_prob], 
                                stdout=subprocess.PIPE, stderr=subprocess.STDOUT, encoding='utf-8')
        print(result.stdout)
        result = subprocess.run([checker_path, edge_paths[i], weight_paths[i], result_path, loss_prob], 
                                stdout=subprocess.PIPE, stderr=subprocess.STDOUT, encoding='utf-8')
        print(result.stdout)