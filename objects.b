class(t) :=

#class varables
value  := t:;
#class functions
get() := 
n:= value:;
	n
:;
set(v) :=
    value == v   
:;

run(command) := #determins what inner fuction to run, allows public and private functions
    if[ not isString(command) ?
        println("input command needs to bea string!")
      ],
    if[ command = "get" ?
        get
      : command = "set" ?
        set
      ]
    :;

run   # this gets run when class is called
:;

t := class(3) :;
println("t value is ",t("get")()),
t("set")(4),
println("t value is ",t("get")()),