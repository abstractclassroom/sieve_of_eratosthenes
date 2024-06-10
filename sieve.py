def sieve_of_eratosthenes(n):
    primes = [True] * (n + 1)
    p = 2
    while p * p <= n:
        if primes[p]:
            for i in range(p * p, n + 1, p):
                primes[i] = False
        p += 1
    
    prime_numbers = [p for p in range(2, n + 1) if primes[p]]
    return prime_numbers

n = 400000000
primes = sieve_of_eratosthenes(n)

# print("List of Prime Numbers Less Than or Equal to", n)
# print(sieve_of_eratosthenes(n))
