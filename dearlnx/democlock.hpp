#ifndef __DEMOCLOCK_HPP__
#define __DEMOCLOCK_HPP__

class DemoClock
{

  public:
    DemoClock(){
      init();
    }
    
    virtual ~DemoClock()
    {
      destroy();
    }
    
    void tick( double delta )
    {
      if( !_paused )
        _timeRef += delta;
    }
    
    void forward( double delta )
    {
      _timeRef += delta;
    }
    
    void backward( double delta )
    {
      _timeRef -= delta;
    }
    
    void pause( bool state )
    {
      _paused = state;
    }
    
    bool paused()
    {
      return _paused;
    }
    
    double time()
    {
      return _timeRef;
    }
    
  private:
    void init()
    {
      _paused = false;
      _timeRef = 0.0;
    }
    
    void destroy()
    {
      
    }
    
    double _timeRef;
    bool   _paused;
    
  protected:
  
};

#endif
