/*
 * MVNException.h
 *
 *  Created on: 24 de fev de 2018
 *      Author: giovcavalc
 */

#ifndef MVNEXCEPTION_H_
#define MVNEXCEPTION_H_

#include <stdexcept>

namespace std {

class MVNException: public logic_error {
public:
	using logic_error::logic_error;
};

} /* namespace std */

#endif /* MVNEXCEPTION_H_ */
