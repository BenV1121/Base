#pragma once

#include "compdef.h"
#include <iostream>



namespace base
{
	
	
class Transform
{
private:
	Transform  *m_parent, *m_children[TRANS_MAX_CHILDREN];
	vec2  m_localPosition;
	vec2  m_localScale;
	float m_localAngle;


	void keepGlobalTransform(Transform *oldParent, Transform *newParent)
	{
		mat3 res = getLocalTransform();
		
		if(oldParent) res = oldParent->getGlobalTransform() * res;
		if(newParent) res = newParent->getGlobalTransform().inverse() * res;
		
		m_localPosition = res.getTrans2D();
		m_localScale	= res.getScale2D();
		m_localAngle	= res.getAngle2D();
	}
public:
	void draw(const mat3 &cam) const
	{
		auto t = cam * getGlobalTransform();

		debugDrawLine(t.c3.xy, t.c2.xy + t.c3.xy,0x00ff00ff);
		debugDrawLine(t.c3.xy, t.c1.xy + t.c3.xy,0xff0000ff);
	}

	Transform() : m_parent(nullptr), m_localScale(1), m_localAngle(0) { memset(m_children, 0, sizeof(m_children)); }

	~Transform()
	{
		// free children
		for (int i = 0; i < TRANS_MAX_CHILDREN; ++i)
			if (m_children[i] != nullptr)
				removeChild(m_children[i], true);
	}

	bool removeChild(Transform *child, bool keepGlobal = false)
	{
		if (!child || child->m_parent != this) return false;

		for (int i = 0; i < TRANS_MAX_CHILDREN; ++i)
			if (m_children[i] == child)
			{
				if (keepGlobal)
					m_children[i]->keepGlobalTransform(this, nullptr);

				m_children[i]->m_parent = nullptr;
				m_children[i] = nullptr;
				return true;
			}
	}

	bool addChild(Transform *child, bool keepGlobal = false)
	{
		if (!child || child->m_parent == this)
			return false;

		for (int i = 0; i < TRANS_MAX_CHILDREN; ++i)
			if(m_children[i] == nullptr)
			{
				if(child->m_parent)
					child->m_parent->removeChild(child, keepGlobal);

				if (keepGlobal)
					m_children[i]->keepGlobalTransform(nullptr, this);

				m_children[i] = child;
				child->m_parent = this;
				return true;
 			}
		return false;
	}

	Transform *getParent() { return m_parent; }

	Transform *getChild(size_t idx) {  return idx < TRANS_MAX_CHILDREN ? m_children[idx] : nullptr; }
	
	// Local Methods
	void setLocalPosition(const vec2 &P) { m_localPosition = P; }
	void setLocalScale(const vec2 &S)    { m_localScale    = S; }
	void setLocalAngle(float a)		     { m_localAngle    = a; }

	const vec2 &getLocalPosition()  const { return m_localPosition; }
	const vec2 &getLocalScale()	  const { return m_localScale;    }
	const float	&getLocalAngle()	  const { return m_localAngle;    }
		  mat3 getLocalTransform()  const { return mat3::translate(m_localPosition)
													   * mat3::rotate(m_localAngle)
													   * mat3::scale(m_localScale);
												}

	
	void setGlobalPosition(const vec2 &P) { m_localPosition = (getGlobalToLocal() * vec3(P, 1)).xy; }
	void setGlobalScale(const vec2 &S)    { m_localScale    = (getGlobalToLocal() * vec3(S, 0)).xy; }
	void setGlobalAngle(float a)				{ m_localAngle    = (getGlobalToLocal() * vec3(vec2::fromAngle(a),0)).xy.angle(); }

	vec2 getGlobalUp() const { return getGlobalTransform().c2.xy.normal(); }
	vec2 getGlobalHorizontal() const { return getGlobalTransform().c1.xy.normal(); }

	vec2 getGlobalPosition()  const { return getGlobalTransform().getTrans2D(); }
	vec2 getGlobalScale()		const { return getGlobalTransform().getScale2D(); }
	float	   getGlobalAngle()		const { return getGlobalTransform().getAngle2D(); }


	mat3 getGlobalTransform() const { return getLocalToGlobal() * getLocalTransform(); }

	// convert points in local space to global space, this is the parent's transform
	mat3 getLocalToGlobal()   const
	{
		return m_parent ? m_parent->getGlobalTransform() : mat3();
	}

	// convert points in global space to local space, this is the parent's inverse
	mat3 getGlobalToLocal()   const {	return getLocalToGlobal().inverse(); }
};


}