#include <stdio.h>
#include <math.h>

double get_sum(double* data[], int len)
{
	double sum=0;
	for(int i=0;i<len;i++)
	{
		sum+=fabs(*data[i]);
	}
	return sum;
}
int normalize(double* data[], int len)
{
	double sum,norm_val;
	sum = get_sum(data,len);
	if(sum==0)
	{
		return 0;
	}
	for(int i=0;i<len;i++)
	{
		norm_val = fabs(*data[i])/sum;
		*data[i]=norm_val;
	}
	return 1;


}