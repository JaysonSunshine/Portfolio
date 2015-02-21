library(kernlab)
data(spam)
head(spam)

#your feature
plot(density(spam$your[spam$type=="nonspam"]), col="blue")
lines(density(spam$your[spam$type=="spam"]), col="red")
abline(v=0.5)

#totalCapital feature
plot(density(spam$capitalTotal[spam$type=="nonspam"]), col="blue")
plot(density(spam[57][spam$type=="nonspam"]), col="blue")
lines(density(spam$capitalTotal[spam$type=="spam"]), col="red")
set.seed(12345)

for(i in 1:length(spam)){
  plot(density(spam[[i]][spam$type=="nonspam"]), col="blue", xlab = i)
  lines(density(spam[[i]][spam$type=="spam"]), col="red")
}
#dollar, hash, your, credit, free, address, people, receive, mail, our, all, make
#how to quantify the above, how to automate it, how to include all of them, computational complexity

range(spam$your[spam$type=="spam"])

spamPredictor <- function(spam){
  vals = seq(0,11.1,by=0.1)
  optimal_pos = 0
  optimal_neg = 0
  pos = 0
  neg = 0
  ns_vals = double()
  ns_sensitivity = double()
  ns_specificity = double()
  s_vals = double()
  steps = double()
  for(i in vals){
    prediction <- ifelse(spam$your > i, "spam", "nonspam")
    x <- table(prediction, spam$type)/length(spam$type)
    ns <- x[1,1] / (x[1,1] + x[1,2])
    ns_sens <- x[1,1] / (x[1,1] + x[2,1])
    ns_spec <- x[2,2] / (x[1,2] + x[2,2])
    ns_vals <- c(ns_vals, ns)
    ns_sensitivity <-c(ns_sensitivity, ns_sens)
    ns_specificity <-c(ns_specificity, ns_spec)
    steps = c(steps, i)
    if(ns > pos){
      pos = ns
      optimal_pos = i
    }
    s <- x[2,2] / (x[2,1] + x[2,2])
    if(s > neg){
      neg = s
      optimal_neg = i
    }
    s_vals <- c(s_vals, s)
  }
  #old.par <- par(mfrow=c(2, 1))
  #dev.new(width = 5, height = 4)
  plot(steps, ns_vals, xlab = "Number of 'your' occurrences",type='l', ylab = "Accuracy",
       col=2, ylim = c(0,1), main = "Positive and negative predictive power" )
  lines(steps, s_vals, col=3)
  legend(0.4,1,c("Positive predictive value", "Negative predictive value"), col=c(2,3), lty=c(1,1), cex =0.7)
  plot(1 - ns_specificity, ns_sensitivity, col="green", type='l', ylim = c(0,1),
       xlab = "1 - specificity", ylab = "Sensitivity", main = "ROC")
  points(steps, steps)
  #lines(steps, ns_specificity, col="orange")
  #par(old.par)
  z = list(optimal_pos, optimal_neg, steps, ns_vals, s_vals)
  return(z)
}
x = spamPredictor(spam)

prediction1 <- ifelse(spam$your > 6, "spam", "nonspam")
prediction1 <- ifelse(spam$capitalTotal > 6, "spam", "nonspam")
table(prediction2, spam$type)/length(spam$type)

plot(spam$your[spam$type=="spam"], col="blue")
points(spam$your[spam$type=="nonspam"], col="red")

plot(spam$capitalTotal[spam$type=="spam"], col="blue")
points(spam$capitalTotal[spam$type=="nonspam"], col="red")
