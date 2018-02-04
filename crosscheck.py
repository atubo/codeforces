#!/usr/bin/python
import string
import random
from random import randint
import time
import os

def rand_str(size, chars=string.ascii_lowercase):
    return ''.join(random.choice(chars) for _ in range(size))

def rand_array(size, lo=-10000, hi=10000):
    ret = []
    for i in range(size):
        ret.append(random.randint(lo, hi))
    return ret

def rand_matrix(row, col, lo=-10000, hi=10000):
    ret = []
    for i in range(row):
        ret.append(rand_array(col, lo, hi))
    return ret

def rand_unique_array(size, lo=1, hi=100000):
    ret = []
    seen = set()
    for i in range(size):
        x = randint(lo, hi)
        while x in seen:
            x = randint(lo, hi)
        ret.append(x)
        seen.add(x)
    return ret

def rand_permutation(size):
    ret = [i for i in range(1, size+1)]
    random.shuffle(ret)
    return ret

def rand_tree(size):
    """return adj array, index from 0 to size-1"""
    adj = [[] for i in range(size)]
    for u in range(1, size):
        v = randint(0, u-1)
        adj[u].append(v)
        adj[v].append(u)
    return adj

def rand_bigint(size):
    ret = chr(ord('0') + randint(1, 9))
    for i in range(size-1):
        ret += chr(ord('0') + randint(0, 9))
    return ret

def print_tree_edges(f, adj):
    # Note here index is from 1
    N = len(adj)
    for u in range(N):
        for v in adj[u]:
            if u < v:
                f.write("%d %d\n" % (u+1, v+1))


def print_weighted_tree_edges(f, adj, lo, hi):
    """Given adj, this function adds random weight and print the tree"""
    # Note here index is from 1
    N = len(adj)
    for u in range(N):
        for v in adj[u]:
            if u < v:
                if randint(1, 2) == 1:
                    u, v = v, u
                w = randint(lo, hi)
                f.write("%d %d %d\n" % (u+1, v+1, w))


def print_array(f, a):
    for x in a:
        f.write("%d " % x)
    f.write("\n")

def print_matrix(f, m):
    for a in m:
        print_array(f, a)

def rand_range(low, hi):
    x = randint(low, hi)
    y = randint(low, hi)
    if x > y:
        x, y = y, x
    return x, y

def swap(s, i, j):
    lst = list(s)
    lst[i], lst[j] = lst[j], lst[i]
    return ''.join(lst)

def generate_input(size):
    os.system("rm -f input.txt")
    with open("input.txt", "w") as f:
        s = 'BBBBBBBWWWWWWWOO'
        for i in range(32):
            x = randint(0, 15)
            y = randint(0, 15)
            s = swap(s, x, y)
        for i in range(4):
            f.write("%s\n" % s[i*4:(i+1)*4])

def one_test(size):
    generate_input(size)
    #exit(0)
    ret = os.system("bench <input.txt> out1.txt")
    if ret != 0:
        print "Bench error"
        exit(1)
    ret = os.system("P2346 <input.txt > out2.txt")
    if ret != 0:
        print "Test error"
        exit(1)
    ret = os.system("diff out1.txt out2.txt")
    return ret == 0

def test_suite():
    for size in range(5, 100):
        print "size = "  + str(size)
        for t in range(1, 100):
            ok = one_test(size)
            if not ok:
                print("diff!")
                exit(0)

test_suite()

#generate_input(250000)
