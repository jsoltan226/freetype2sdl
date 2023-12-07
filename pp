#define d ((float)m->horiBearingY)
#define f (d / (float)((int)lineH << 6))
int result = 1 - f - ((float)(face->ascender >> 6) / lineH);
