#pragma once
#ifndef H_SWAPMVTP
#define H_SWAPMVTP

#include "Codelet.h"
#include "ThreadedProcedure.h"

class Swap : public darts::Codelet
{
public:
  Swap () {}
  Swap (uint32_t dep, uint32_t res, darts::ThreadedProcedure * myTP, uint32_t stat);
  virtual void fire ();
};

#endif
