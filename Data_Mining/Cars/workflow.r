library(treemap)
library(scales)

#change path of output_for_R as appropriate
sample_output <- read.table("<path>/output_for_R", quote="\"")
head(sample_output)

scaled_sample_output = sample_output
scaled_sample_output["V4"] = rescale(sample_output["V4"], to=c(0, 100000))
treemap(scaled_sample_output, index = c("V3", "V2"),
        vSize = "V5", vColor = "V4", type = "value")

#change path of output_for_R as appropriate
Final_Paid_TDB <- read.csv("<path>/Final_Paid_TDB.txt", header=FALSE)
head(Final_Paid_TDB)

cor_new_mpg_old_mpg = cor(Final_Paid_TDB$V41, Final_Paid_TDB$V27)
cor_new_mpg_old_mpg
cor_new_mpg_new_msrp = cor(Final_Paid_TDB$V41, Final_Paid_TDB$V42)
cor_new_mpg_new_msrp #higher mpg means lower cost
lm(Final_Paid_TDB$V41 ~ Final_Paid_TDB$V27)#new mpg to old mpg
lm(Final_Paid_TDB$V42 ~ Final_Paid_TDB$V27)#msrp to old mpg

#Survey of Consumer Response to CARS Initiative

#change path of output_for_R as appropriate
consumer.survey <- read.csv("<path>/consumer-survey.csv", header=FALSE)
head(consumer.survey)
consumer_length = 143998

yes_traded = sum(consumer.survey$V1, na.rm = TRUE)
yes_traded
no_traded = sum(consumer.survey$V2, na.rm = TRUE)
no_traded

soon_traded = sum(consumer.survey$V3, na.rm = TRUE) + sum(consumer.survey$V4, na.rm = TRUE)
soon_traded

long_traded = 0
for(i in 8:14){
  long_traded = long_traded + sum(consumer.survey[i], na.rm = TRUE)
}
long_traded

#Question 2
purchase_no = sum(consumer.survey$V15, na.rm = TRUE)
purchase_no

purchase_new = sum(consumer.survey$V16, na.rm = TRUE)
purchase_new

purchase_used = sum(consumer.survey$V32, na.rm = TRUE)
purchase_used

category = c('Trade-in without CARS:Yes', 'Trade-in without CARS:No', 'Trade-in within one year', 'Trade-in >= 5 years', 'Not purchase without incentive', 'Purchase new vehicle', 'Purchase used vehicle')
values = c(yes_traded, no_traded, soon_traded, long_traded, purchase_no, purchase_new, purchase_used)
percent = values / c(rep(consumer_length,2), rep(no_traded, 2), rep(consumer_length, 3))
survey = data.frame(category, values, percent)
survey

options(digits = 2)
library(gridExtra)
grid.table(survey)
