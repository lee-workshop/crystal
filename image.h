#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <X11/Xlib.h>
#include <X11/Xmu/WinUtil.h>
#include "log.h"

class Image {
public:
	Image();
	Image(const int w, const int h, const unsigned char *rgb,
			const unsigned char *alpha);

	~Image();

	const unsigned char *getPNGAlpha() const {
		return(png_alpha);
	};
	const unsigned char *getRGBData() const {
		return(rgb_data);
	};

	void getPixel(double px, double py, unsigned char *pixel);
	void getPixel(double px, double py, unsigned char *pixel,
					unsigned char *alpha);

	int Width() const {
		return(width);
	};
	int Height() const {
		return(height);
	};
	void Quality(const int q) {
		quality_ = q;
	};

	bool Read(const char *filename);

	void Reduce(const int factor);
	void Resize(const int w, const int h);
	void Merge(Image *background, const int x, const int y);
	void Merge_non_crop(Image* background, const int x, const int y);
	void Crop(const int x, const int y, const int w, const int h);
	void Tile(const int w, const int h);
	void Center(const int w, const int h, const char *hex);
	void Plain(const int w, const int h, const char *hex);

	void computeShift(unsigned long mask, unsigned char &left_shift,
				unsigned char &right_shift);

	Pixmap createPixmap(Display *dpy, int scr, Window win);

private:
	int width, height, area;
	unsigned char *rgb_data;
	unsigned char *png_alpha;

	int quality_;

	int readJpeg(const char *filename, int *width, int *height,
		unsigned char **rgb);
	int readPng(const char *filename, int *width, int *height,
		unsigned char **rgb, unsigned char **alpha);
};

#endif /* _IMAGE_H_ */
