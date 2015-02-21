library(caret)
consumer.survey <- read.csv("~/Desktop/School/Jobs/Edmunds/consumer-survey.csv", header=FALSE)
consumer.survey[is.na(consumer.survey)] <- 0
head(consumer.survey)

inTrain <- createDataPartition(y=consumer.survey$V1, p=0.6, list=FALSE)

training <-consumer.survey[inTrain,]
testing <- consumer.survey[-inTrain,]

modelFit <- train(V1 ~., data=training, method="glm")
modelFit

modelFit$finalModel

predictions <- predict(modelFit, newdata=testing)
predictions

confusionMatrix(predictions, testing$type)
