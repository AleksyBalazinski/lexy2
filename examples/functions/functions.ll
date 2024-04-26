source_filename = "./examples\functions\functions.l2"
target triple = "x86_64-w64-windows-gnu"

@formatInt = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@formatDouble = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1

declare i32 @printf(ptr, ...)

define dso_local i32 @b.4.1(i32 noundef %x) #0 {
  %x.4.1.1 = alloca i32
  store i32 %x, i32* %x.4.1.1
  %retVal = alloca i32
  %1 = load i32, i32* %x.4.1.1
  %2 = add i32 %1, 2
  store i32 %2, i32* %retVal
  %3 = load i32, i32* %retVal
  ret i32 %3
}

define dso_local i32 @a.4(i32 noundef %x) #0 {
  %x.4.1 = alloca i32
  store i32 %x, i32* %x.4.1
  %retVal = alloca i32
  %1 = load i32, i32* %x.4.1
  %call8 = call i32 @b.4.1(i32 noundef %1)
  %2 = load i32, i32* %x.4.1
  %3 = add i32 %2, %call8
  store i32 %3, i32* %retVal
  %4 = load i32, i32* %retVal
  ret i32 %4
}

define dso_local i32 @very_nested(i32 noundef %x) #0 {
  %x.4 = alloca i32
  store i32 %x, i32* %x.4
  %retVal = alloca i32
  %1 = load i32, i32* %x.4
  %call9 = call i32 @a.4(i32 noundef %1)
  %2 = load i32, i32* %x.4
  %3 = add i32 %2, %call9
  store i32 %3, i32* %retVal
  %4 = load i32, i32* %retVal
  ret i32 %4
}

define dso_local i1 @is_odd.3(i32 noundef %x) #0 {
  %x.3.2 = alloca i32
  store i32 %x, i32* %x.3.2
  %retVal = alloca i8
  %1 = load i32, i32* %x.3.2
  %2 = srem i32 %1, 2
  %3 = icmp eq i32 %2, 1
  %4 = zext i1 %3 to i8
  store i8 %4, i8* %retVal
  %5 = load i1, i1* %retVal
  ret i1 %5
}

define dso_local i1 @is_even.3(i32 noundef %x) #0 {
  %x.3.1 = alloca i32
  store i32 %x, i32* %x.3.1
  %retVal = alloca i8
  %1 = load i32, i32* %x.3.1
  %2 = srem i32 %1, 2
  %3 = icmp eq i32 %2, 0
  %4 = zext i1 %3 to i8
  store i8 %4, i8* %retVal
  %5 = load i1, i1* %retVal
  ret i1 %5
}

define dso_local i32 @foo(i32 noundef %start, i32 noundef %end, i32 noundef %step) #0 {
  %start.3 = alloca i32
  store i32 %start, i32* %start.3
  %end.3 = alloca i32
  store i32 %end, i32* %end.3
  %step.3 = alloca i32
  store i32 %step, i32* %step.3
  %retVal = alloca i32
  %sum.3 = alloca i32
  store i32 0, i32* %sum.3
  %1 = load i32, i32* %start.3
  %i.3 = alloca i32
  store i32 %1, i32* %i.3
  br label %while.cond1
while.cond1:
  %2 = load i32, i32* %i.3
  %3 = load i32, i32* %end.3
  %4 = icmp sle i32 %2, %3
  br i1 %4, label %while.body1, label %while.end1
while.body1:
  %5 = load i32, i32* %i.3
  %call5 = call i1 @is_even.3(i32 noundef %5)
  br i1 %call5, label %if.then2, label %if.end2
if.then2:
  %6 = load i32, i32* %i.3
  %7 = load i32, i32* %sum.3
  %8 = add i32 %7, %6
  store i32 %8, i32* %sum.3
  br label %if.end2
if.end2:
  %9 = load i32, i32* %i.3
  %call6 = call i1 @is_odd.3(i32 noundef %9)
  br i1 %call6, label %if.then3, label %if.end3
if.then3:
  %10 = load i32, i32* %i.3
  %11 = mul i32 2, %10
  %12 = load i32, i32* %sum.3
  %13 = add i32 %12, %11
  store i32 %13, i32* %sum.3
  br label %if.end3
if.end3:
  %14 = load i32, i32* %step.3
  %15 = load i32, i32* %i.3
  %16 = add i32 %15, %14
  store i32 %16, i32* %i.3
  br label %while.cond1
while.end1:
  %17 = load i32, i32* %sum.3
  store i32 %17, i32* %retVal
  %18 = load i32, i32* %retVal
  ret i32 %18
}

define dso_local i32 @seq_sum(i32 noundef %start, i32 noundef %end, i32 noundef %step) #0 {
  %start.2 = alloca i32
  store i32 %start, i32* %start.2
  %end.2 = alloca i32
  store i32 %end, i32* %end.2
  %step.2 = alloca i32
  store i32 %step, i32* %step.2
  %retVal = alloca i32
  %sum.2 = alloca i32
  store i32 0, i32* %sum.2
  %1 = load i32, i32* %start.2
  %i.2 = alloca i32
  store i32 %1, i32* %i.2
  br label %while.cond
while.cond:
  %2 = load i32, i32* %i.2
  %3 = load i32, i32* %end.2
  %4 = icmp sle i32 %2, %3
  br i1 %4, label %while.body, label %while.end
while.body:
  %5 = load i32, i32* %i.2
  %6 = load i32, i32* %sum.2
  %7 = add i32 %6, %5
  store i32 %7, i32* %sum.2
  %8 = load i32, i32* %step.2
  %9 = load i32, i32* %i.2
  %10 = add i32 %9, %8
  store i32 %10, i32* %i.2
  br label %while.cond
while.end:
  %11 = load i32, i32* %sum.2
  store i32 %11, i32* %retVal
  %12 = load i32, i32* %retVal
  ret i32 %12
}

define dso_local i1 @and(i1 noundef %x, i1 noundef %y) #0 {
  %1 = zext i1 %x to i8
  %x.1 = alloca i8
  store i8 %1, i8* %x.1
  %2 = zext i1 %y to i8
  %y.1 = alloca i8
  store i8 %2, i8* %y.1
  %retVal = alloca i8
  %3 = zext i1 false to i8
  %result.1 = alloca i8
  store i8 %3, i8* %result.1
  %4 = load i8, i8* %x.1
  %5 = trunc i8 %4 to i1
  br i1 %5, label %if.then, label %if.end
if.then:
  %6 = load i8, i8* %y.1
  %7 = trunc i8 %6 to i1
  br i1 %7, label %if.then1, label %if.end1
if.then1:
  %8 = zext i1 true to i8
  store i8 %8, i8* %result.1
  br label %if.end1
if.end1:
  br label %if.end
if.end:
  %9 = load i8, i8* %result.1
  %10 = trunc i8 %9 to i1
  %11 = zext i1 %10 to i8
  store i8 %11, i8* %retVal
  %12 = load i1, i1* %retVal
  ret i1 %12
}

define dso_local i32 @main() #0 {
  %call = call i1 @and(i1 noundef false, i1 noundef false)
  %1 = zext i1 %call to i32
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %1)
  %call1 = call i1 @and(i1 noundef true, i1 noundef false)
  %3 = zext i1 %call1 to i32
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %3)
  %call2 = call i1 @and(i1 noundef false, i1 noundef true)
  %5 = zext i1 %call2 to i32
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %5)
  %call3 = call i1 @and(i1 noundef true, i1 noundef true)
  %7 = zext i1 %call3 to i32
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %7)
  %call4 = call i32 @seq_sum(i32 noundef 4, i32 noundef 13, i32 noundef 2)
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %call4)
  %call7 = call i32 @foo(i32 noundef 2, i32 noundef 14, i32 noundef 1)
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %call7)
  %call10 = call i32 @very_nested(i32 noundef 1)
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %call10)
  ret i32 0
}
