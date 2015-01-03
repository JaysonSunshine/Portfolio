################################################################
# Stats102c/Wu                                 UCLA ID 504209098
# Date 1 May 2014
# Homework 2
################################################################

#Problem 1, Part 4
#Using the work from Part 1
expected_x_4 = function(n){
  X = rnorm(n)
  Y = 1 / n * sum(X^4)
  return(Y)
}

#Problem 2, Part 3
f = function(x) sqrt(2/pi)*exp(-x^2/2)
E_2 = function(n){
  x = rexp(n,2)
  w = f(x) / pexp(4,2)
  return(1/n*sum(x*w))
}
E_2(10000)

#Problem 3, Part 1
f = function(x) dnorm(x)
g = function(x,c) exp(-(x-c))
E = function(n,c) {
  x = rexp(n) + c #same as last homework
  w = f(x)/g(x,c)
  I = mean(w) #or use sum(w)/n
  return(I)
}
E(100, 4); 1 - pnorm(4)

#Problem 3, Part 2
f = function(x1,x2,x3, mu) {
  
  dnorm(x1, mu, 1) * dnorm(x2, mu, 1) * dnorm(x3, mu, 1)
  
}
x1 = rnorm(N, alpha, 1)
x2 = rnorm(N, alpha, 1)
x3 = rnorm(N, alpha, 1)
X = cbind(x1 = x1, x2 = x2,x3 = x3)

# generate X i.i.d from normal
#M = apply(X, 1, FUN = function(x) {max(x[1], x[1]+x[2],     
                                  #     x[1]+x[2]+x[3])}) # "apply" means to apply the FUN to each row of X. 1 means apply to row. see ?apply for more details.
h = function(X, C) {
  
  M = maply(X, .fun = function(x1, x2, x3) {max(x1, x1+x2, x1+x2+x3)}, 
            
            .expand = FALSE)
  return(M > C)
  
} # this function returns a vector consists of TRUEs and FALSEs, which equal to 1 and 0 when taken into computing

#Now you have f, M, h. You need to define your own g and the rest. 

#Let's try for mu = -1
I_fraction = function(N, mu, alpha, C){
  x1 = rnorm(N, alpha, 1)
  x2 = rnorm(N, alpha, 1)
  x3 = rnorm(N, alpha, 1)
  X = cbind(x1 = x1, x2 = x2,x3 = x3)
  w = f(x1,x2,x3, mu) / pnorm(X, alpha)
  z = h(X,C)
  return(sum(z * w) / N)
  #return(length(z[z == TRUE])/N)
}

I_fraction(1000, 1, -1, 1) #about 1/1000 have value of true

#Problem 4.2
normal_dist = function(n){
  theta = 2*pi*runif(n)
  r = sqrt(-2 * log(runif(n)))
  x = r*cos(theta)
  y = r*sin(theta)
  z = list("x" = x,"y" = y)
  return(z)
}
z = normal_dist(10000)
hist(z$x)
hist(z$y)
qqnorm(z$x)
qqnorm(z$y)
