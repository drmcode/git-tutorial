#wildcard ==> exposion all wildcard files
#VPATH variable parameter VPATH xxx:yyy means include xxx files, yyy files
#vpath keywords of make, usage: vpath <pattern> <directories>
# <pattern> usally using "%" means zero of n characters.
# eg: vpath %.h ../headers

#.PHONY 

# 自动化变量 	  ## 说明 ##
#   $@ 	    表示规则的目标文件名。如果目标是一个文档文件（Linux 中，一般成 .a 文件为文档文件，也成为静态的库文件），
#           那么它代表这个文档的文件名。在多目标模式规则中，它代表的是触发规则被执行的文件名。
#   $% 	    当目标文件是一个静态库文件时，代表静态库的一个成员名。
#   $< 	    规则的第一个依赖的文件名。如果是一个目标文件使用隐含的规则来重建，则它代表由隐含规则加入的第一个依赖文件。
#   $? 	    所有比目标文件更新的依赖文件列表，空格分隔。如果目标文件时静态库文件，代表的是库文件（.o 文件）。
#   $^ 	    代表的是所有依赖文件列表，使用空格分隔。如果目标是静态库文件，它所代表的只能是所有的库成员（.o 文件）名。
#           一个文件可重复的出现在目标的依赖中，变量“$^”只记录它的第一次引用的情况。就是说变量“$^”会去掉重复的依赖文件。
#   $+ 	    类似“$^”，但是它保留了依赖文件中重复出现的文件。主要用在程序链接时库的交叉引用场合。
#   $* 	    在模式规则和静态模式规则中，代表“茎”。“茎”是目标模式中“%”所代表的部分（当文件名中存在目录时，
#           “茎”也包含目录部分）。


CC=g++
#-MMD for output dependence to *.d files
#-fPIC not related with location
#-Wformat
#-Wformat-security 检擦格式安全，如果要漏掉某些错误可以用 -Wno-xxxx 如： -Wno-format-truncation -Wno-invalid-offsetof
#CXXFLAGS=-Wall -std=c++11 -Wformat -Wformat-security -Werror -MMD -fPIC 
CXXFLAGS=-Wall -std=c++11 -Wformat -Wformat-security -Werror -MMD -fPIC
#CXXFLAGS+=-g #-g for gdb debug
LDFLAGS=-lpthread
TAGET=fire

OBJS=fire.o sp.o internalmsghandler.o internalmsgqu.o collectrain.o fire_mem.o
SRC=fire.cpp sp.cpp internalmsghandler.cpp internalmsgqu.cpp collectrain.cpp fire_mem.cpp
INC=fire.hpp sp.hpp commTypes.hpp sp_debug.hpp collectrain.hpp IRain.hpp fire_mem.hpp #internalmsghandler.h imsghandler.h iservice.h internalmsgqu.h

INC+=*.h

all : $(TAGET)

$(TAGET) : $(OBJS)
	@echo start compile ...
	$(CC) $^ -o $@ $(LDFLAGS)
	chmod a+x $@

$(OBJS) : $(SRC) $(INC)
	$(CC) $(CXXFLAGS) -c $^
#$(CC) $(CPPFLAGS) -c $^ -o $@ 

.PHONY:
	clean

clean:
	-rm $(OBJS) $(TAGET) *.d *.gch
