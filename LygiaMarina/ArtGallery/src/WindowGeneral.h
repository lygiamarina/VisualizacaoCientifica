#include <stdio.h>

class WindowGeneral
{
private:
	static WindowGeneral* currentInstance;

	int *size;
	int *position;
	char *title;
	float *backgroundColor4Float;
	
public:
	WindowGeneral();
	WindowGeneral(char *title);
	WindowGeneral(int *size, int *position, float* backgroundColor4Float);
	~WindowGeneral();

	//Screen
	void initialize();
	void drawBackground();
	static void drawCallback();
	void drawDisplay();

	//Get and Set
	void setSize(int *size);
	int* getSize();

	void setPosition(int *position);
	int* getPosition();

	void setTitle(char *title);
	char* getTitle();

	void setBackgroundColor4Float(float *backgroundColor4Float);
	float* getBackgroundColor4Float();

	//Common methods
	void deleteObj();
};
