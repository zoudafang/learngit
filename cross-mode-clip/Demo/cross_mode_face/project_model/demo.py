import torch
import numpy as np
import os
import argparse
from ICMR import Solver
import time

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--query', type=str, default='a woman with glasses.')
    parser.add_argument('--dataset', type=str, default='celebA', help='Dataset name: celabA, mirflickr, mscoco, nus-wide')
    parser.add_argument('--epoch', type=int, default=50, help='default:50 epochs')
    parser.add_argument('--hash_lens', type=int , default=128)
    parser.add_argument('--device', type=str , default="cuda:0", help='cuda device')
    parser.add_argument('--task', type= int, default= 1, help="0 is train real; 1 is train hash; 2 is test real; 3 is test hash")
    config = parser.parse_args()
    print(config)
    solver = Solver(config)
    solver.inference(config.query)