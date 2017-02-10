#include "stdafx.h"
#include "help.h"

void OptionHelp()
{
	TCHAR select;
	printf("�鿴����ѡ�������˵��(Y/N):");
	scanf("%c", &select);
	if (select == 'y' || select == 'Y')
		help();
}

void help()
{
	printf("\n����ָ��ģʽ(��-mѡ��ָ��)�����޸ĵ��ļ�(��-fѡ��ָ��)\n");
	printf("���ܵ�ѡ������:\n");
	printf("chppara\n");
	printf("chlpara\n");
	printf("att2para\n");
	printf("att2note\n");
	printf("\n");
}

void updatecoordLoopHelp()
{
	printf("�÷���\nmapgistool -m updatecoord -d ����·�� [-x xfield -y yfield] -f �ļ��� ...\n\n");
	printf("һ�οɸ��¶��mapgis���ļ�x,y���꣬\"-x, -y\" ѡ��Ϊ��ѡ������ Ĭ��д��\"XX, YY\" �ֶΣ���Ϊ˫�����ͣ���\n\nʾ���÷����£�\n");
	printf("mapgistool -m updatecoord -d C:\\L4821 -f gpoint.wt photo.wt attitude.wt\n");
	printf("����ʾ����ʾ����gpoint.wt, photo.wt attitude.wt �������ļ���x,y ���꣬�������Ӧ�ֶ�Ĭ�ϣ���XX, YY��\n");
	OptionHelp();
}

void updateroutingHelp()
{
	printf("�÷���\nmapgistool -m updaterouting -d ����·�� -f routing.wl\n\n");
	printf("mapgistool -m updaterouting -d C:\\L4821 -f routing.wl\n");
	OptionHelp();
}

void chpparaHelp()
{
	printf("�÷���\nmapgistool -m chppara -d ����·�� -f �ļ��� [��Ҫ�޸ĵĲ��� ����ֵ] ...\n\n");
	printf("һ��ֻ�ܹ��޸�һ���ļ�����[��Ҫ�޸ĵĲ��� ����ֵ] ...��\n�����Կո�ָ�������ɶ�ָ��������һ�ԡ�\n\nʾ���÷����£�\n");
	printf("mapgistool -m chppara -d C:\\L4821 -f gpoint.wt he 3 wi 4\n");
	printf("\n��ʾ��gpoint.wt�ļ�����ͼ�ġ��߶ȡ�����Ϊ3������ȡ�����Ϊ4��\nhe, wi ���������ַ���ǰ2λ���޸���������ʱȡ��Ӧ������ǰ2λ����\n");
	OptionHelp();
}

void chlparaHelp()
{
	printf("�÷���\nmapgistool -m chlpara -d ����·�� -f �ļ��� [��Ҫ�޸ĵĲ��� ����ֵ] ...\n");
	printf("һ��ֻ�ܹ��޸�һ���ļ�����[��Ҫ�޸ĵĲ��� ����ֵ] ...����ʾ�����Կո�ָ�������ɶԳ�������һ�ԡ�\nʾ���÷����£�\n");
	printf("mapgistool -m chppara -d C:\\L4821 -f BOUNDARY.WL lt 1 lc 6 lw 0.25\n");
	printf("\t��ʾ���ļ�BOUNDARY.WL��������(lt)�޸�Ϊ1������ɫ(lc)�޸�Ϊ6,�߿�(lw)�޸�Ϊ0.25,\n");
	OptionHelp();
}


void att2paraHelp()
{
	printf("�÷���\nmapgistool -m att2para -d ����·�� -f �ļ��� �����ֶ� ��Ӧ����ֵ [��Ҫ�޸ĵĲ��� ����ֵ] ...\n");
	printf("һ��ֻ�ܹ��޸�һ���ļ���[�����ֶ� ��Ӧ����ֵ]ֻ������һ�ԡ�\n[��Ҫ�޸ĵĲ��� ����ֵ] ...����ʾ�����Կո�ָ�������ɶԳ�������һ�ԡ�ʾ���÷����£�\n");
	printf("mapgistool -m att2para -d C:\\L4821 -f BOUNDARY.WL TYPE ���ѽ��� lt 1 lc 6 lw 0.25\n");
	printf("\t��ʾBOUNDARY.WL�ļ���\"TYPE\"����Ϊ\"���ѽ���\"����.\n��������(lt)�޸�Ϊ1������ɫ(lc)�޸�Ϊ6,�߿�(lw)�޸�Ϊ0.25,\n");
	OptionHelp();
}

void att2noteHelp()
{
	printf("�÷���\nmapgistool -m att2note -d ����·�� -f �ļ��� [-p �����ļ���] ���ɱ�ע���ļ��� ��Ҫ��Ϊ��ע���ֶ� [...]\n");
	printf("һ��ֻ������һ���ļ��ı�ע������Ҫ��Ϊ��ע���ֶ� [...]����ʾ����ʹ�ö���ֶ�������Ϊ��ע��\nʾ���÷����£�\n");
	printf("mapgistool -m att2note -d C:\\L4821 -f gpoint.wt -p L4821.mpj GPTNOTE.WT GPOINT ID");
	printf("\n��ʾ��gpoint.wt�ļ��е�GPOINT, ID�ֶζ�Ӧ���������ɱ�ע(���ɵı�ע��ʽΪD1234-2(GPOINT-ID))\n");
	printf("��ע�ļ���Ϊ:GPTNOTE.WT,�ұ�ע�ļ������ӵ������ļ�L4821.mpj�С�\n");
	printf("������ɵı�ע�������룬����������ɱ�ע�����ֶβ�Ϊ�ַ����й�\n");
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