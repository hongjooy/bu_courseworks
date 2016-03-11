# AUTHOR1: Jooyoun Hong hongjooy@bu.edu

%Jooyoun Hong hongjooy@bu.edu

1. [Sums, 20 pts]

Provide a closed-form solution to the following sums. Please explain your answer.

(a) $$\sum_{i=0}^{\infty} \frac{1}{3^i}$$

Solution: $$\sum_{i=0}^{\infty} \frac{1}{3^i} = \lim_{n\to\infty} \sum_{i=0}^{n} \frac{1}{3^i} = \lim_{n\to\infty}\frac{1(\frac{1}{3}^n - 1)}{\frac{1}{3} - 1} = \frac{3}{2}$$ 

(b) $$\sum_{i=4}^{N} 5^i$$
Solution: $$\sum_{i=4}^{N} 5^i = \frac{5^4(5^N-1)}{4}$$
(c) $$\sum_{i=0}^{N} (i^3 + 9i-2)$$
Solution: $$\sum_{i=0}^{N} (i^3 + 9i-2) = \sum_{i=0}^{N}(i^3) + 9\sum_{i=0}^{N}(i) - 2N = (\frac{N(N+1)}{2})^2 + 9\frac{N(N+1)}{2} - 2N = \frac{N^4+2N^3+19N^2+10N}{4}$$
(d) $$\sum_{i=1}^{10^N} \log_5 i$$
Solution: $$\sum_{i=1}^{10^N} \log_5 i = \log_5 1 + \log_5 2 + ... +\log_5 (10)^N  = \log_5 (1*2*3*...*10^N) = \log_5(10^N!)$$


2. [Exponents and logs, 20 pts]

(a) $x\cdot x^2 \cdot x^3 \cdot \ldots \cdot x^N$
Solution:  $x\cdot x^2 \cdot x^3 \cdot \ldots \cdot x^N = x^{1+2+...+N }= x^{\frac{N(N+1)}{2}}$

(b) $\log_n (n^6)$
Solution: $\log_n (n^6) = 6$

(c) $17^{\log_{17} 280}$
Solution: $$17^{\log_{17} 280} = 280^{\log_{17} 17} = 280$$

(d) $\ln(\ln(e^{e^N}))$
Solution $\ln(\ln(e^{e^N})) = \ln(e^N) = N$

3. [Combinatorics, 10 pts]

(a) How many options are there to award gold, silver, and bronze medals to a group of 10 athletes.
$$_{10}P_{3} = 720$$
(b) How many ways are there to pick four increasing numbers from a list of numbers 1 through 39?
$${39\choose 4} = 82251$$




