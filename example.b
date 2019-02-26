#Bee language example file 


# This is a comment that gets ignored until a new line
## this gets ignored until new line or # println("this is visible):;

#this is how varables are set
i := 44:;

# ':;' is required after defining the varables
# changing or calling funtions require a ',' at the end
println("test"),

# this is an int
a :=44:;

#this is a string
b:= "string":;

#this is a bool
c:= true:;

#this is a real
d:= 1.0:;

#assignment is done with '=='
d == 2.0,

#you dont need to specify type to change 
d == "hello",

#functions are first class objects and can be set the same way as varables
f() := 1+1:;

#function with input
jj(x) := 1+x:;

#lambdas are also supported
lambda[x,y: x^2 + y^2],

#functions can contain lambdas
jjj := lambda[x,y:  y + x] :;

#arrays start at zero and are allocated with built-in "allocate"
aa := allocate(5) :; #ranges from 0-4


#arrays are accessed with square brackets after the varable name containing the array
aa[0] == 1,

#same with varables you dont need to specify type 
aa[0] == "a",
aa[0] == 3,

#lists

#if statements are with the call 'if'
if[ 1 < 2 ? true ],

x;
# if statements can be assigned to names
x == if[ 1 < 2 ? true ],

x == 1,
# more than one variables can be use and also the catch all 'else'
x == if[ x < 0 ?
         "first"
       : x = 0 ?
         "second"
       : x > 0 ?
         "third"
       : else 
         "else"
       ],

# while loops have both a pre and post condtional actions  with the condtion in the middle
while[println("pre action") : i < 5 : println("post action",i == i + 1) ],


#blocks are available to seperate scopes
ii := 2 :;
    	{
	        bb := 5 :;
	        ii == ii * bb
            #bb reachable
    	},
#bb not reachable


# some funtions are built in to make life easier here are a few of the most used

#printing a line and printing a line with a new line after
print("test, "),
println("test2"),

#cdr , cons, car , lists
LL := list(1,2,3,4):;
LR := list(1,2,3,4):;
println(LL),

println(car(LL)),
println(cdr(LL)),
println(cons(LL,LR)),

#type change

g :=1.0:;

g == integer(g),

#sizeof


#type
I:= 1:;

if[ not isString(I) ? println("FALSE!"): else  println("TRue!" )],
if[ not isInteger(I) ? println("FALSE!"): else println("TRue!" )],
if[ not isNil(I) ? println("FALSE!"): else  sprintln("TRue!") ],
if[ not isReal(I) ? println("FALSE!"): else  println("TRue!" )],



#command line access 

println(getArgCount()),
println(getArg(1)),


# file access

	#open
	fp := openFile("./data.ints"):;

	#read
	println(readInteger(fp)),

	#close
	closeFile(fp),

