#ifndef RENDERABLE_H_
#define RENDERABLE_H_

/**
 * An abstract class for rendering and updating
 */
class Renderable{
public:
	Renderable(){};

	virtual ~Renderable(void){};

	/**
	 * Can be rendered
	 */
	virtual void draw() =0;

	/**
	 * Can be updated
	 */
	virtual void update() =0;

};

#endif
