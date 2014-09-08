#pragma once

#include <initializer_list>

struct APP_InterfaceBase {
	virtual uint16_t size() const =0;
	virtual void publish(){
		uint8_t i;

		//Before beginning the copy we have to take the mutex, but because
		//we actually copy the WHOLE instance the mutex will be copied as well
		//so we'll loose our handle to the shared structure mutex. Therefore we
		//save it before the copy:
		Mutex shared_mutex(shared->mutex);

		shared_mutex.take();
		for(i=0; i<this->size(); i++){
			((uint8_t*)shared)[i] = ((uint8_t*)this)[i];
		}
		shared_mutex.release();

		//Now the mutex handle in the actual shared structure has been overwritten!
		//Therefore we will just reassign it:
		shared->mutex = shared_mutex;
	}

	virtual void receive(){
		uint8_t i;
		uint16_t size = this->size();

		//Before beginning the copy we have to take the mutex, but because
		//we actually copy the WHOLE instance the mutex will be copied as well
		//HOWEVER this doesn't matter because we'll never actually use the mutex
		//of a local container.
		shared->mutex.take();

		for(i=0; i<size; i++){
			((uint8_t*)this)[i] = ((uint8_t*)shared)[i];
		}

		shared->mutex.release();
	}

	APP_InterfaceBase* shared = 0;
	Mutex mutex;
};

template<typename T>
struct APP_Interface : APP_InterfaceBase {

	APP_Interface(){
		shared = this;
	}

	APP_Interface(std::initializer_list<T*> list){
		shared = this;
	    for( auto elem : list )
	    {
	    	elem->shared = (T*)this;
	    }
	}

   // Overload + operator to bind interfaces
   virtual T operator+(T* a)
   {
	  a->shared = this->shared;
	  return *a;
   }

	virtual uint16_t size() const { return sizeof(T); }


};



