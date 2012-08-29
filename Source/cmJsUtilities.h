/*=========================================================================
  Copyright (c) 2009 Peter Kümmel
  All rights reserved.
  See Copyright.txt or http://www.cmake.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef CMJSUTILITIES_H
#define CMJSUTILITIES_H

#include <QtScript>


namespace QScriptBind
{

	template<class T>
	QScriptValue scriptValueFromValue(QScriptEngine* engine, const T& t)
	{
		// deep copy into script value
		const QByteArray bytes(reinterpret_cast<const char*>(&t), sizeof(T));
		return engine->newVariant(QVariant(bytes));
	}


	template<class T>
	T valueFromScriptValue(const QScriptValue& value)
	{
		const QByteArray bytes = value.toVariant().toByteArray();
		T t = *reinterpret_cast<T*>(const_cast<char*>(bytes.constData()));
		return t;
	}


	void registerGlobalFunction(QScriptEngine* engine, const QString& name, QScriptEngine::FunctionSignature func);

	void exportDefinitionToJs(QScriptEngine* engine, const char* name);



	// bind non-QObject classes

	// needed for the const char* specialisation
	template<class T>
	struct qscriptvalue
	{	
		qscriptvalue(const QScriptValue& v)  : value(v) {}
		T cast() { return qscriptvalue_cast<T>(value); }
	private: 
		const QScriptValue& value;
	};


	template<>
	struct qscriptvalue<const char*>
	{
		// byte array must exists as long as someone uses the const char* data
		qscriptvalue(const QScriptValue& value)  : bytes(value.toString().toLatin1()) {}
		const char* cast() { return bytes.constData(); }
	private: 
		const QByteArray bytes;
	};


	template<typename R>
	struct call
	{
		template<class T, typename P1>
		static QScriptValue memberFunction(T& t, R (T::* const func)(P1) const, QScriptContext* context)
		{
			R r = (t.*func)(qscriptvalue<P1>(context->argument(0)).cast());
			return QScriptValue(context->engine(), r);
		}
	};


	template<>
	struct call<void>
	{
		template<class T, typename P1, typename P2>
		static QScriptValue memberFunction(T& t, void (T::* const func)(P1, P2), QScriptContext* context)
		{
			(t.*func)(
				qscriptvalue<P1>(context->argument(0)).cast(),
				qscriptvalue<P2>(context->argument(1)).cast()
				);
			return QScriptValue();
		}
	};


	template<class T = void>
	struct bind
	{
		template<typename R, typename P1>
		static inline QScriptValue memberFunction(T& t, R (T::* const func)(P1) const, QScriptContext* context)
		{
			return call<R>::memberFunction(t, func, context);
		}

		template<typename P1, typename P2>
		static inline QScriptValue memberFunction(T& t, void (T::* const func)(P1, P2), QScriptContext* context)
		{
			return call<void>::memberFunction(t, func, context);
		}
	};


	template<class T = void> 
	struct closure
	{
		template<typename Func>
		static QScriptValue function(QScriptContext* context, QScriptEngine *engine)
		{
			T* t = valueFromScriptValue<T*>(context->thisObject().data());
			Func f = valueFromScriptValue<Func>(context->callee().data());
			return bind<T>::memberFunction(*t, f, context);
		}
	};


	template<class T = void>
	class Class : public QScriptValue
	{
	public:
		Class(T* t, QScriptEngine* engine)
		{
			// make it a Object
			QScriptValue::operator=(engine->newObject());
			setData(scriptValueFromValue(engine, t));
		}


		template<typename Func>
		inline Class& addMemberFunction(const QString& name, const Func func) 
		{
			QScriptValue memfunc = engine()->newFunction(&closure<T>:: template function<Func>);
			memfunc.setData(scriptValueFromValue<Func>(engine(), func));
			setProperty(name, memfunc);
			return *this;
		}

	};


}



#endif



