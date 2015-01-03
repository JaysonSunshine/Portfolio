  ################################################################
# Stat 102C/Wu                                   UCLA ID 504209098
# Date 17 Apr 2014
# Homework 1
##################################################################

#2.3
random_exp = function(n,x){
  return(-1/x*log(1 - runif(n)))
}

#2.4
z = random_exp(10000,1)
hist(z, main = "Histogram of z (lamda = 1)")

#4.3
truncated_normal = function(n,c){
  M = 2*exp(1/2-c*2/4-c*sqrt(c^2+4)/4)/(sqrt(2*pi)*(1-pnorm(c))*(c+sqrt(c^2+4)))
  Y = rexp(n,rate=(c + sqrt(c^2+4))/2) + c
  Z = runif(n)
  X = rep('na',n)
  for(i in 1:n){
    #if(Y[i] > c)
    if(M*Z[i] <= (dnorm(Y[i])/(1-pnorm(c)))/(dexp(Y[i] - c,rate=(c + sqrt(c^2+4))/2)))
    {
      X[i] = Y[i] + c
    }
  }
  return(X)
 }

x = truncated_normal(10000,3)
hist(as.numeric(x[x != 'na']))
sum(x != 'na')
c = 3
M = 2*exp(1/2-c*2/4-c*sqrt(c^2+4)/4)/(sqrt(2*pi)*(1-pnorm(c))*(c+sqrt(c^2+4)))
1/M * 10000
  
x = truncated_normal(10000,5)
hist(as.numeric(x[x != 'na']))
sum(x != 'na')
c = 5
M = 2*exp(1/2-c*2/4-c*sqrt(c^2+4)/4)/(sqrt(2*pi)*(1-pnorm(c))*(c+sqrt(c^2+4)))
1/M * 10000
