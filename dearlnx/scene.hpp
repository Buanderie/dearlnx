#ifndef __SCENE_HPP__
#define __SCENE_HPP__

class Scene
{
  public:
    Scene(){};
    virtual ~Scene(){};
    
    virtual void load()=0;
    virtual void init()=0;
    virtual void draw( double globalTime )=0;
    
  private:
  
  protected:
  
};

#endif
