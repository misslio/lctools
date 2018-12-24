#include <stdio.h>

/**
 * s means start
 * e means end
 **/
int compute_days(int y_s, int m_s, int d_s, int y_e, int m_e, int d_e)
{
	int y2, m2, d2;
	int y1, m1, d1;

	m1 = (m_s + 9) % 12; /* 距离3月有几个月 */
	y1 = y_s - m1/10; /* 1,2月不计算当年 */
	d1 = 365*y1 + y1/4 - y1/100 + y1/400 + (m1*306 + 5)/10 + (d_s - 1);

	m2 = (m_e + 9) % 12;
	y2 = y_e - m2/10;
	d2 = 365*y2 + y2/4 - y2/100 + y2/400 + (m2*306 + 5)/10 + (d_e - 1);

	return (d2 - d1);
}
