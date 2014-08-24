#include <stdio.h>

class WindowGeneral
{
private:
	int *size;
	int *position;
	char *title;
	float *backgroundColor4Float;
	
public:
	WindowGeneral(char *title="Untitled");
	WindowGeneral(const WindowGeneral &window);
	~WindowGeneral();

	//Screen
	void initialize();
	void drawBackground();

	//Get and Set
	void setSize(int width, int height);
	int* getSize();

	void setPosition(int xPosition, int yPosition);
	int* getPosition();

	void setTitle(char *title);
	char* getTitle();

	void setBackgroundColor4Float(float R, float G, float B, float alpha);
	float* getBackgroundColor4Float();

	//Common methods
	void deleteObj();
};
