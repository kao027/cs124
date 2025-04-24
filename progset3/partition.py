import heapq
import numpy as np
import math
import sys
max_iter = 10000

def karp(nums):
  for i in range(len(nums)): # negating values so it acts like max heap
    nums[i] = -nums[i]
  heapq.heapify(nums)
  while len(nums) > 1:
    a = heapq.heappop(nums)
    b = heapq.heappop(nums)
    heapq.heappush(nums, a-b)
  return -nums[0]


def residue(S,A):
  return abs(np.dot(S,A))

def partition(A, P):
    sums = np.zeros(max(P) + 1)
    for a, p in zip(A, P):
        sums[p] += a
    return sums


def partition_karp(A, P):
    karp_result = karp(list(partition(A, P)))
    return karp_result

def repeated_random(A):
  length = len(A)
  S = np.random.choice([-1, 1], size=length)
  for i in range(max_iter):
    S_prime = np.random.choice([-1, 1], size=length)
    if residue(S_prime, A) < residue(S, A):
      S = np.copy(S_prime)
  return residue(S,A)

def partition_repeated_random(A):
  length = len(A)
  P = np.random.randint(1, length, length)
  #print(f"P:{P}")
  for i in range(max_iter):
    P_prime = np.random.randint(1, length, length)
    if partition_karp(A, P_prime) < partition_karp(A, P):
      P = np.copy(P_prime)
  return abs(partition_karp(A,P))

def random_neighbor(S_prime, length):
    i = np.random.randint(0, length)
    j = np.random.randint(0, length)
    while j == i:
      j = np.random.randint(0, length)
    S_prime[i]=-S_prime[i]
    if (np.random.choice([0, 1])==1): #50% chance
       S_prime[j]=-S_prime[j]
    return S_prime

def partition_random_neighbor(P):
    n = len(P)
    i = np.random.randint(0, n)
    j = np.random.randint(0, n)
    while P[i] == j:
        j = np.random.randint(0, n)
    P_new = np.copy(P)
    P_new[i] = j
    return P_new

def T(iter):
  return 10**10*(0.8)**math.floor(iter/300)

def simulated_annealing(A):
  length = len(A)
  S = np.random.choice([-1, 1], size=length)
  S_double_prime = np.copy(S)
  for iter in range(1, max_iter + 1):
    S_prime = random_neighbor(S, length)
    if residue(S_prime, A) < residue(S, A):
      S = np.copy(S_prime)
    else:
      prob = math.exp(-(residue(S_prime, A) - residue(S, A))/T(iter))
      if np.random.rand() < prob:
        S = np.copy(S_prime)
    if residue(S_prime, A) < residue(S_double_prime, A):
      S_double_prime = np.copy(S_prime)
  return residue(S_double_prime,A)

def partition_simulated_annealing(A):
  length = len(A)
  P = np.random.randint(1, length, length)
  P_double_prime = np.copy(P)
  for iter in range(1, max_iter + 1):
    P_prime = partition_random_neighbor(P)
    #print(P_prime, P)
    if partition_karp(A, P_prime) < partition_karp(A, P):
      P = np.copy(P_prime)
    else:
      prob = math.exp(-(partition_karp(A, P_prime) - partition_karp(A, P))/T(iter))
      if np.random.rand() < prob:
        P = np.copy(P_prime)
    if partition_karp(A, P_prime) < partition_karp(A, P_double_prime):
      P_double_prime = np.copy(P_prime)
  return partition_karp(A, P_double_prime)

def hillclimb(A):
  length = len(A)
  S = np.random.choice([-1,1], size=(length))
  for _ in range(max_iter):
    S_prime = np.copy(S)
    random_neighbor(S_prime, length)
    if residue(S_prime, A) < residue(S, A):
      S = np.copy(S_prime)
  return residue(S,A)

def partition_hillclimb(A):
  length = len(A)
  P = np.random.randint(1, length, length)
  for i in range(max_iter):
    P_prime = np.copy(P)
    P_prime = partition_random_neighbor(P_prime)
    if partition_karp(A, P_prime) < partition_karp(A,P):
      P = np.copy(P_prime)
  return abs(partition_karp(A,P))

def generate_list(filename):
  infile = open(filename, "r")
  A = np.empty(0)
  for _ in range(100):
    A = np.append(A, int(infile.readline()))
  return A

def run_algo(A, algo):
  if (algo==0):
    return abs(karp(list(A)))
  if (algo==1):
    return repeated_random(A)
  if (algo==2):
    return hillclimb(A)
  if (algo==3):
    return simulated_annealing(A)
  if (algo==11):
    return partition_repeated_random(A)
  if (algo==12):
    return partition_hillclimb(A)
  if (algo==13):
    return partition_simulated_annealing(A)
  else:
    print("Invalid algorithm code.")
    return -1



def main(args):
  if len(args)!=3 and len(args)!=1:
    print("Error: Wrong number of argumens")
  flag = int(args[0])
  global max_iter
  if (flag == 0):
    max_iter = 10000
    algo = int(args[1])
    filename = args[2]
    nums = generate_list(filename)
    print(int(run_algo(nums,algo)))
  elif (flag == 1):
    max_iter = 25000
    num_trials = 50
    algo_codes = [0,1,2,3,11,12,13]
    for code in algo_codes:
      avg_residue = 0
      for _ in range(num_trials):
        nums = np.random.randint(1, 10**12, 100)
        avg_residue += run_algo(nums, code)
      avg_residue /= num_trials
      print(f"Average Algorithm {code} Residue: {avg_residue}")

  else:
    print("Flag should either be 0 or 1")


if __name__ == "__main__":
    main(sys.argv[1:])