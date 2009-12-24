//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions
//ICanAssociate.h - Author Jon Pry 

#ifndef ICANASSOCIATE_H
#define ICANASSOCIATE_H

class ICanAssociate
{
protected:
    const ICanAssociate* m_parent;
public:
    ICanAssociate(const ICanAssociate* parent):m_parent(parent){}
    ICanAssociate(){}

    const ICanAssociate* GetParent(){return m_parent;}
};

#endif