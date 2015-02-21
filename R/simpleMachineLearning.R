library(caret)
library(kernlab)
data(spam)

inTrain <- createDataPartition(y=spam$type, p=0.6, list=FALSE)

training <-spam[inTrain,]
testing <- spam[-inTrain,]

#optional folds
folds <- createFolds(y=spam$type, k=10, list=TRUE, returnTrain=TRUE)

#folds with resampling
folds <- createResample(y=spam$type, times=10, list=TRUE)

#folds with time slices
tme <- 1:1000
folds <- createTimeSlices(y=tme, initialWindow=20,horizon=10)
  
modelFit_glm <- train(type ~., data=training, method="glm")
modelFit_glm
modelFit_gbm <- train(type ~., data=training, method="gbm")
modelFit_gbm
modelFit_svm <- train(type ~., data=training, method="svmLinear")
modelFit_svm
modelFit_svmpoly <- train(type ~., data=training, method="svmPoly")
modelFit_rf <- train(type ~., data=training, method="rf")
modelFit_rf
modelFit_nnet <- train(type ~., data=training, method="nnet")

modelFit
modelFit$finalModel

predictions <- predict(modelFit, newdata=testing)
predictions

confusionMatrix(predictions, testing$type)
