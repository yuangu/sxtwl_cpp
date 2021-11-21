package sxtwl

/*
// 参考：https://kaisawind.gitee.io/2020/10/24/2020-10-24-cgo-build/
#cgo windows LDFLAGS:-L${SRCDIR}/../lib/win64  -lsxtwl_go -lsxtwl -lstdc++ -Wl,-rpath=./
#cgo linux LDFLAGS:-L${SRCDIR}/../lib/linux  -lsxtwl_go -lsxtwl -lm  -lstdc++ -Wl,-rpath=./
*/
import "C"
