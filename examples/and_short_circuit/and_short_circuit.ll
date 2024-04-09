source_filename = "./examples\and_short_circuit\and_short_circuit.l2"
target triple = "x86_64-w64-windows-gnu"

@formatInt = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@formatDouble = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1

declare i32 @printf(ptr, ...)

define dso_local i32 @main() #0 {
entry:
  %q = alloca i32
  store i32 12, i32* %q
  %z = alloca i32
  store i32 21, i32* %z
  %0 = load i32, i32* %q
  %1 = mul i32 %0, 2
  %2 = load i32, i32* %q
  %3 = sub i32 %2, 4
  %4 = add i32 %1, %3
  %5 = load i32, i32* %q
  %6 = sub i32 %5, 3
  %7 = sdiv i32 %6, 3
  %8 = mul i32 %4, %7
  %9 = sub i32 %8, 95
  %10 = icmp eq i32 %9, 1
  br i1 %10, label %and.rhs, label %and.end
and.rhs:
  %11 = load i32, i32* %z
  %12 = sdiv i32 %11, 3
  %13 = add i32 3, 4
  %14 = sub i32 %12, %13
  %15 = mul i32 %14, 100
  %16 = icmp eq i32 %15, 0
  br label %and.end
and.end:
  %17 = phi i1 [false, %entry], [%16, %and.rhs]
  br i1 %17, label %if.then, label %if.end
if.then:
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef 1)
  br label %if.end
if.end:
  %19 = mul i32 3, 4
  %20 = load i32, i32* %q
  %21 = icmp eq i32 %20, %19
  br i1 %21, label %and.rhs2, label %and.end2
and.rhs2:
  %22 = mul i32 7, 3
  %23 = load i32, i32* %z
  %24 = icmp eq i32 %22, %23
  br label %and.end2
and.end2:
  %25 = phi i1 [false, %if.end], [%24, %and.rhs2]
  br i1 %25, label %and.rhs1, label %and.end1
and.rhs1:
  br label %and.end1
and.end1:
  %26 = phi i1 [false, %and.end2], [true, %and.rhs1]
  %27 = zext i1 %26 to i8
  %myBool = alloca i8
  store i8 %27, i8* %myBool
  %28 = load i8, i8* %myBool
  %29 = trunc i8 %28 to i1
  %30 = zext i1 %29 to i32
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %30)
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef 1000)
  %x = alloca i32
  store i32 100, i32* %x
  %y = alloca i32
  store i32 15, i32* %y
  %a = alloca i32
  store i32 3, i32* %a
  br label %while.cond
while.cond:
  %33 = load i32, i32* %x
  %34 = icmp sgt i32 %33, 0
  br i1 %34, label %and.rhs4, label %and.end4
and.rhs4:
  %35 = load i32, i32* %y
  %36 = icmp sgt i32 %35, 0
  br label %and.end4
and.end4:
  %37 = phi i1 [false, %while.cond], [%36, %and.rhs4]
  br i1 %37, label %and.rhs3, label %and.end3
and.rhs3:
  %38 = load i32, i32* %a
  %39 = icmp ne i32 %38, 150
  br label %and.end3
and.end3:
  %40 = phi i1 [false, %and.end4], [%39, %and.rhs3]
  br i1 %40, label %while.body, label %while.end
while.body:
  %41 = load i32, i32* %x
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %41)
  %43 = load i32, i32* %x
  %44 = sub i32 %43, 1
  store i32 %44, i32* %x
  %45 = load i32, i32* %y
  %46 = sub i32 %45, 2
  store i32 %46, i32* %y
  br label %while.cond
while.end:
  br i1 true, label %and.rhs8, label %and.end8
and.rhs8:
  br label %and.end8
and.end8:
  %47 = phi i1 [false, %while.end], [true, %and.rhs8]
  br i1 %47, label %and.rhs7, label %and.end7
and.rhs7:
  br label %and.end7
and.end7:
  %48 = phi i1 [false, %and.end8], [false, %and.rhs7]
  br i1 %48, label %and.rhs6, label %and.end6
and.rhs6:
  br label %and.end6
and.end6:
  %49 = phi i1 [false, %and.end7], [true, %and.rhs6]
  br i1 %49, label %and.rhs5, label %and.end5
and.rhs5:
  br label %and.end5
and.end5:
  %50 = phi i1 [false, %and.end6], [true, %and.rhs5]
  %51 = zext i1 %50 to i32
  call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef %51)
  ret i32 0
}
