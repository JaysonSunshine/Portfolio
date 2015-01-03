#Problem 4
#a

plot(a$food, a$cost, main = "Food(x) Cost(y)")
plot(a$decor, a$cost, main = "Decor(x) Cost(y)")
plot(a$ser, a$cost, main = "Ser(x) Cost(y)")

#b
c_f <- lm(a$cost ~ a$food)
summary(c_f)
c_d <- lm(a$cost ~ a$decor)
summary(c_d)
c_s <- lm(a$cost ~ a$ser)
summary(c_s)

#c
#Cost is most correlated to  decor: r^2(cost,decor) = 0.7619 > r^2(cost,food) and r^2(cost,ser)

#d
summary(c_d)
#if I understand the question correctly, so far we've only done regression on one variable, and I selected decore, so food rating and service rating have no influence on cost
c_d$coefficients[1] + 16 * c_d$coefficients[2]

#e
total = a$food + a$decor + a$ser
plot(total, a$cost, main = "Total(x) Cost(y)")

#f
c_t <- lm(a$cost ~ total)
summary(c_t)
#The r^2 is 0.827, greater than correlation between any single variable
#cost = -42.38 + 1.23 * total

#g
plot(fitted(c_t), c_t$residuals, main = "Fitted values(x) Residuals(y)")
plot(total, c_t$residuals, main = "Total(x) Residuals(y)")
#the variance of residuals does not appear to increase as either the fitted values or the total value increases, consistent with the assumptions of our model

#h
#I interpret this question as prompting us for sample ser, food, and decor values, computing total, and then using the model from f
#for my restaurant, I don't care much about decor so I'll say 15, ser is somewhat important, so 18, and food is important so 20, giving a total of 53

c_t$coefficients[1] + 53 * c_t$coefficients[2]
#value is $22.98

#i
#If you include the cost, service, and food quality of a restaurant we can account for ~83% of the variation in cost, which makes for a good model. Interesting, the food had the least r^2, implying people care more about appearances and how they're treated (both service and decor had higher r^2 values).

#Problem 5
#a

a <- read.table("http://www.stat.ucla.edu/~nchristo/statistics100C/restaurant.txt", header=TRUE)
ones <- rep(1,nrow(a))
x <- as.matrix(cbind(ones, a$food, a$decor, a$ser))

#b
b_hat <- solve(t(x) %*% x) %*% t(x) %*% a$cost

#c
c <- lm(a$cost ~ a$food + a$decor +a$ser)
summary(c)
#values are identical to b_hat

#d
H <- x %*% solve(t(x) %*% x) %*% t(x)
H[1:5,1:5]
