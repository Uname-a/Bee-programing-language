#file reading of ints


sum := 0:;
fp := openFile("./data.ints"):;
i := 0:;
i == readInteger(fp),
sum == sum + i,
while[
    i == readInteger(fp),
    sum == sum + i
   :not(atFileEnd(fp)) : 
  ],
println("sum is ",sum),
println("sum should be 2045"),
closeFile(fp),
