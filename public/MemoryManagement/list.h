/*
 * list
 *
 *  Created on: 17 de jul de 2020
 *      Author: breno
 */

#ifndef _MEMORYMANAGEMENT_LIST_
#define _MEMORYMANAGEMENT_LIST_

#include <MemoryManagement/new.h>
#include <MemoryManagement/StaticAllocator.h>
#include <RTOSCodeStyle.h>

namespace RTOS
{
    template<class dataType, class alocatorParam>
    class list
    {
        template<class listNodeDataType>
        class listNode
        {
        public:

            typedef listNode<listNodeDataType> listNodeDef;

            listNode() :
                    _next(nullptr), _previous(nullptr), _data()
            {
            }

            listNode(const listNodeDataType data) :
                    _next(nullptr), _previous(nullptr), _data(data)
            {
            }

            listNode(const listNodeDataType data, const listNodeDef *next) :
                    _next(next), _previous(nullptr), _data(data)
            {
            }

            listNode(const listNodeDataType data, const listNodeDef *next, const listNodeDef *previous) :
                    _next(next), _previous(previous), _data(data)
            {
            }

            listNodeDataType& data()
            {
                return _data;
            }

            void data(const listNodeDataType &data)
            {
                _data = data;
            }

            listNodeDef* next()
            {
                return _next;
            }

            void next(listNodeDef *next)
            {
                _next = next;
            }

            listNodeDef* previous()
            {
                return _previous;
            }

            void previous(listNodeDef *previous)
            {
                _previous = previous;
            }

            bool isEnd()
            {
                return _next == nullptr;
            }
            bool isBegin()
            {
                return _previous == nullptr;
            }

        private:
            listNodeDef *_next;
            listNodeDef *_previous;
            listNodeDataType _data;
        };
    public:

        template<class listIteratorDataType>
        class listIterator
        {
        public:

            listIterator(listNode<listIteratorDataType>* const node)
            {
                _node = node;
            }

            listIterator operator++()
            {
                if (!_node->isEnd())
                {
                    _node = _node->next();
                }
                return *this;
            }

            listIterator operator--()
            {
                if (!_node->isBegin())
                {
                    _node = _node->previous();
                }
                return *this;
            }

            bool operator==(const listIterator &IteratorB) const
            {
                return _node == IteratorB._node;
            }

            bool operator!=(const listIterator &IteratorB) const
            {
                return _node != IteratorB._node;
            }

            dataType& operator*()
            {
                return _node->data();
            }

            listNode<listIteratorDataType>* node()
            {
                return _node;
            }

        private:
            listNode<listIteratorDataType> *_node;
        };

        typedef listIterator<dataType> Iterator;

        typedef typename alocatorParam::template  rebind<listNode<dataType>>::other alocatorType;

        listNode<dataType> *_begin;
        listNode<dataType> *_end;
        alocatorType alocator;
        uint8 _size;

        list()
        {
            _begin = nullptr;
            _end = nullptr;
            _size = 0;

            listNode<dataType> *newNode = alocator.allocate(1);
            new(newNode) listNode<dataType>();

            _end = newNode;
            _begin = newNode;
        }

        dataType front()
        {
            return _begin->data();
        }

        listIterator<dataType> begin()
        {
            return _begin;
        }

        listIterator<dataType> end()
        {
            return _end;
        }

        void push_front(const dataType &value)
        {
            listNode<dataType> *newNode = createNode(value);

            if (!empty())
            {
                _begin->previous(newNode);
                newNode->next(_begin);
            }
            else
            {
                _end->previous(newNode);
                newNode->next(_end);
            }

            _begin = newNode;

            _size++;
        }

        listNode<dataType>* createNode(const dataType &value)
        {
            listNode<dataType> *newNode = alocator.allocate(1);
            alocator.construct(newNode, value);
            return newNode;
        }

        void deleteNode(listNode<dataType> *node)
        {
            alocator.destroy(node);
            alocator.deallocate(node);
        }

        void push_back(const dataType &value)
        {
            listIterator<dataType> iterator(_end);
            --iterator;
            insert(iterator, value);
        }

        void insert(listIterator<dataType> &pos, const dataType &value)
        {
            if (pos == _end)
            {
                if (empty())
                {
                    push_front(value);
                }
                else
                {
                    push_back(value);
                }
            }
            else
            {
                if (pos != nullptr && pos.node() != nullptr)
                {
                    listNode<dataType> *newNode = createNode(value);
                    listNode<dataType> *previousNode = pos.node();

                    // previous state A<->B desired A<->C<->B
                    newNode->previous(previousNode); //A<-C


                    listNode<dataType> *nextNode = pos.node()->next();
                    nextNode->previous(newNode); //C<-B
                    newNode->next(nextNode); //C->B

                    previousNode->next(newNode); // A->C
                    _size++;
                }
                else
                {
                    push_front(value);
                }
            }
        }

        uint8 max_size()
        {
            return alocator.max_size();
        }

        uint8 size()
        {
            return _size;
        }

        bool empty()
        {
            return _size == 0;
        }

        void pop_back()
        {
            listIterator<dataType> iterator(_end);
            --iterator;
            remove(_end);
        }

        void pop_front()
        {
            listIterator<dataType> iterator(_begin);
            remove(iterator);
        }

        void remove(listIterator<dataType> &pos)
        {
            if (pos != nullptr && pos.node() != nullptr && pos != _end)
            {
                listNode<dataType> *removedNode = pos.node();



                    if (removedNode->isBegin())
                    {
                        _begin = removedNode->next();
                        _begin->previous(nullptr);
                    }


                deleteNode(removedNode);
                --_size;
            }
        }

        void swap(const listIterator<dataType> &posA, const listIterator<dataType> &posB)
        {
            listNode<dataType> *NodeA = posA.node();
            listNode<dataType> *NodeB = posB.node();

            changeOtherReferences(NodeA, NodeB);
            changeOtherReferences(NodeB, NodeA);

            listNode<dataType> *NodeANext = NodeA->next();
            listNode<dataType> *NodeAPrevious = NodeA->previous();

            NodeA->next(NodeB->next());
            NodeA->previous(NodeB->previous());

            NodeB->next(NodeANext);
            NodeB->previous(NodeAPrevious);
        }

        void changeOtherReferences(listNode<dataType> *nodeA,
                listNode<dataType> *nodeB)
        {
            nodeA->next()->next(nodeB);
            nodeA->previous()->previous(nodeB);
        }

    };
}
;

#endif /* _MEMORYMANAGEMENT_LIST_ */
