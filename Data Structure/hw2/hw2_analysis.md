%AUTHOR1: Andy Shen shena@bu.edu
%AUTHOR2: Jooyoun Hong hongjooy@bu.edu

Asymptotics, 25 pts
===================

Solution:
$$0 =  \prod\limits_{k=1}^{n}(1-\frac{1}{k^3}) < 1 = n^\frac{1}{n} =  (1+\frac{1}{n})^n < \log n <  \sqrt[3]{n} < \frac{n}{4} = \sum\limits_{k=1}^{\log n} \frac{n}{3^k} < n^3 < n^9+n+2 < (n+1)^n < n^{n^n}$$

The ordering starts with 0, which is the smallest. The product is next because the first product in the series is $1-(1/k^3)$ where k is 1 so that is 0. Anything else after that, no matter what n is, is 0. Next is 1, which is a constant. $n^{1/n}$ and $(1+\frac{1}{n})^n$ converges to 1 as well. Next is logn, which asymptotically larger than a constant. After that is n raised to the power of a fraction. Next, $n/4$ is equal to the summation because both are n over a fraction. Then we have n raised to a power, where n^3 is less than n^9. That is all less than $(1+\frac{1}{n})^n$ because the exponential is n. This is still an order of n less than $(n+1)^n$. The highest asymptotically is $n^{n^n}$.

Recurrences, 25 pts
===================
Solutions :
( a )
  $$T(n)=3* T(n-1) +c$$
   Annihilators --> $$T(n+1)-3T(n) = 0$$.
Therefore, O($3^n$)

( b )
For n > 1, B(n//2)>=5. 
$$T(n)=2*T(n/2)+c$$. Larger factor n only matters. Therefore, O(n)

( c )
$T(n)=\sqrt{n}*T(\sqrt{n})+\sqrt{n}$ ( called $\sqrt{n}$ times, and runs $\sqrt{n}$ times.) 
   
   Let $n=2^i$. Then,  $$T(2^i)=2^\frac{i}{2}T(2^\frac{i}{2})+2^\frac{i}{2}.$$ Let  $S(i)=T(2^i)$. Then, 
   $$S(i)=2^\frac{i}{2}S(\frac{i}{2})+2^\frac{i}{2}.$$
      $$=2^i(\frac{1}{2}+\frac{1}{4}+\frac{1}{8}+...+\frac{1}{log2(i)})S(1)+2^i(\frac{1}{2}+\frac{1}{4}+\frac{1}{8}+...+\frac{1}{log2(i)})$$
      $$=\frac{2^i}{\sqrt{2}}S(1)+s(i)$$ 
      
Where $$s(i) = \sum_{j=2}^{log2(i+1)}2^\frac{i}{j}$$,
the maximum case of $s(i)$ is when $j = 2$ given $(\log_2{i} + 1)*2^\frac{i}{2}$.

The minimum case of $s(i)$ is when $j=\log_2{i}+1$, given as $(\log_2{i}+1)2^\frac{i}{\log_2{i}+1}$

Plugin $n = 2^i$, the lower bound is  Ω $(n+\sqrt{n}log2(log2(n)))$, and the upper bound O$(n+n\log_2({\log_2{n}}))$.     
  
( d ) 
O$(n\log_2{n})$ is operated twice. Therefore, the complexity is 
 O($n*(\log_2{n})$)
  
( e )  
 $$T(n)=T(n-1)+2T(n-2)+c$$, and $$T(n+2)-T(n+1)-2T(n) = 0$$. 
 Annihilator -->  $E^2-E-2=0$, $E_1=2, E_2=-1$. Larger one 2 only matters. Therefore, O($2^n$)