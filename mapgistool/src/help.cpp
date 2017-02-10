#include "stdafx.h"
#include "help.h"

void OptionHelp()
{
	TCHAR select;
	printf("查看关于选项的其它说明(Y/N):");
	scanf("%c", &select);
	if (select == 'y' || select == 'Y')
		help();
}

void help()
{
	printf("\n必须指定模式(由-m选项指定)和所修改的文件(由-f选项指定)\n");
	printf("接受的选项如下:\n");
	printf("chppara\n");
	printf("chlpara\n");
	printf("att2para\n");
	printf("att2note\n");
	printf("\n");
}

void updatecoordLoopHelp()
{
	printf("用法：\nmapgistool -m updatecoord -d 工程路径 [-x xfield -y yfield] -f 文件名 ...\n\n");
	printf("一次可更新多个mapgis点文件x,y坐标，\"-x, -y\" 选项为可选参数， 默认写入\"XX, YY\" 字段（且为双精度型）。\n\n示例用法如下：\n");
	printf("mapgistool -m updatecoord -d C:\\L4821 -f gpoint.wt photo.wt attitude.wt\n");
	printf("以上示例表示更新gpoint.wt, photo.wt attitude.wt 三个点文件的x,y 坐标，坐标的相应字段默认，即XX, YY。\n");
	OptionHelp();
}

void updateroutingHelp()
{
	printf("用法：\nmapgistool -m updaterouting -d 工程路径 -f routing.wl\n\n");
	printf("mapgistool -m updaterouting -d C:\\L4821 -f routing.wl\n");
	OptionHelp();
}

void chpparaHelp()
{
	printf("用法：\nmapgistool -m chppara -d 工程路径 -f 文件名 [需要修改的参数 参数值] ...\n\n");
	printf("一次只能够修改一个文件，“[需要修改的参数 参数值] ...”\n二者以空格分隔，必须成对指定，至少一对。\n\n示例用法如下：\n");
	printf("mapgistool -m chppara -d C:\\L4821 -f gpoint.wt he 3 wi 4\n");
	printf("\n表示将gpoint.wt文件中子图的“高度”设置为3，“宽度”设置为4。\nhe, wi 意义下列字符的前2位，修改其它参数时取相应参数的前2位即可\n");
	OptionHelp();
}

void chlparaHelp()
{
	printf("用法：\nmapgistool -m chlpara -d 工程路径 -f 文件名 [需要修改的参数 参数值] ...\n");
	printf("一次只能够修改一个文件，“[需要修改的参数 参数值] ...”表示二者以空格分隔，必须成对出，至少一对。\n示例用法如下：\n");
	printf("mapgistool -m chppara -d C:\\L4821 -f BOUNDARY.WL lt 1 lc 6 lw 0.25\n");
	printf("\t表示将文件BOUNDARY.WL的线类型(lt)修改为1，线颜色(lc)修改为6,线宽(lw)修改为0.25,\n");
	OptionHelp();
}


void att2paraHelp()
{
	printf("用法：\nmapgistool -m att2para -d 工程路径 -f 文件名 属性字段 对应属性值 [需要修改的参数 参数值] ...\n");
	printf("一次只能够修改一个文件，[属性字段 对应属性值]只能设置一对。\n[需要修改的参数 参数值] ...”表示二者以空格分隔，必须成对出，至少一对。示例用法如下：\n");
	printf("mapgistool -m att2para -d C:\\L4821 -f BOUNDARY.WL TYPE 断裂界线 lt 1 lc 6 lw 0.25\n");
	printf("\t表示BOUNDARY.WL文件中\"TYPE\"属性为\"断裂界线\"的线.\n将线类型(lt)修改为1，线颜色(lc)修改为6,线宽(lw)修改为0.25,\n");
	OptionHelp();
}

void att2noteHelp()
{
	printf("用法：\nmapgistool -m att2note -d 工程路径 -f 文件名 [-p 工程文件名] 生成标注的文件名 需要作为标注的字段 [...]\n");
	printf("一次只能生成一个文件的标注，“需要作为标注的字段 [...]”表示可以使用多个字段属性作为标注。\n示例用法如下：\n");
	printf("mapgistool -m att2note -d C:\\L4821 -f gpoint.wt -p L4821.mpj GPTNOTE.WT GPOINT ID");
	printf("\n表示将gpoint.wt文件中的GPOINT, ID字段对应的属性生成标注(生成的标注形式为D1234-2(GPOINT-ID))\n");
	printf("标注文件名为:GPTNOTE.WT,且标注文件将增加到工程文件L4821.mpj中。\n");
	printf("如果生成的标注产生乱码，则可能与生成标注依据字段不为字符型有关\n");
	OptionHelp();
}

void dgsattdbHelp()
{
	fprintf(stderr, "usage:\nmapgistool -m dgsattdb -d mapgisdir [-g|-r] -b prbdb.accdb -t gpoint.wt routing.wl ...\n");
	fprintf(stderr, "must specify either -g or -r option\n");
	exit(2);
}

void genegpsHelp()
{
	printf("usage:\n");
	printf("mapgistool -m gengps -d C:\\L4821 -z 8004TERL.WL -s 2016-6-6/7:12:1 -e 2016-6-6/17:34:1\n");
	printf("make sure following conditions are satisfied:\n");
	printf("1: orign gps.wt file exists and not empty;\n");
	printf("2: free.wl, which used to copy timestamp;\n");
	printf("3: file 'line.wl' which used to generate point;\n");
	printf("4: terl file's(8004TERL.WL) altitude field is ELEV and its correspoing type is double.\n");
	printf("5: time must specifes as the usage format\n");
	printf("6: the directory name(e.g. L4821) is used as the routecode attributes of the generated gps.wt file\n");
	OptionHelp();
}