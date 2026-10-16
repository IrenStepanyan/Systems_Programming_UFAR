#ifndef ALTERNANCE_H
#define ALTERNANCE_H

extern void initialiserAlternance();
extern void attendreTour();
extern void donnerTour(pid_t aQui);

extern void traitementPremier(pid_t pidSecond, const char *mess);
extern void traitementSecond(pid_t pidPremier, const char *mess);

#endif
    
      
