
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */

#include <ngx_config.h>
#include <ngx_core.h>

ngx_array_t *
ngx_array_create(ngx_pool_t *p, ngx_uint_t n, size_t size)
{
    ngx_array_t *a;

    /*在内存池pool上面分配一段内存给ngx_array数据结构*/
    a = ngx_palloc(p, sizeof(ngx_array_t));
    if (a == NULL)
    {
        return NULL;
    }

    /*数组初始化，并且分配内存空间给数据元素*/
    if (ngx_array_init(a, p, n, size) != NGX_OK)
    {
        return NULL;
    }

    return a;
}

void ngx_array_destroy(ngx_array_t *a)
{
    ngx_pool_t *p;

    p = a->pool;

    /**
     * 如果数组元素的末尾地址和内存池pool的可用开始的地址相同
     * 则将内存池pool->d.last移动到数组元素的开始地址，相当于清除当前数组的内容
     */

    if ((u_char *)a->elts + a->size * a->nalloc == p->d.last)
    {
        p->d.last -= a->size * a->nalloc;
    }

    /**
     * 如果数组的数据结构ngx_array_t的末尾地址和内存池pool的可用开始地址相同
     * 则将内存池pool->d.last移动到数组元素的开始地址，相当于清除当前数组的内容
     */
    if ((u_char *)a + sizeof(ngx_array_t) == p->d.last)
    {
        p->d.last = (u_char *)a;
    }
}

void *
ngx_array_push(ngx_array_t *a)
{
    void *elt, *new;
    size_t size;
    ngx_pool_t *p;

    /* 如果数组中元素都用完了 ，则需要对数组进行扩容 */
    if (a->nelts == a->nalloc)
    {

        /* the array is full */

        size = a->size * a->nalloc;

        p = a->pool;

        /**
         * 扩容有两种方式
         * 1.如果数组元素的末尾和内存池pool的可用开始的地址相同，
         * 并且内存池剩余的空间支持数组扩容，则在当前内存池上扩容
         * 2. 如果扩容的大小超出了当前内存池剩余的容量或者数组元素的末尾和内存池pool的可用开始的地址不相同，
         * 则需要重新分配一个新的内存块存储数组，并且将原数组拷贝到新的地址上
         */
        if ((u_char *)a->elts + size == p->d.last && p->d.last + a->size <= p->d.end)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.last += a->size;
            a->nalloc++;
        }
        else
        {
            /* allocate a new array */

            /* 重新分配一个 2*size的内存块 */
            new = ngx_palloc(p, 2 * size);
            if (new == NULL)
            {
                return NULL;
            }
            /* 内存块拷贝，将老的内存块拷贝到新的new内存块上面  */
            ngx_memcpy(new, a->elts, size);
            a->elts = new;  /* 内存块指针地址改变 */
            a->nalloc *= 2; /* 分配的个数*2 */
            // size 不变（最大容量，即单个元素大小 x 最大元素数）
            // pool 不变（因为分配新的内存块的时候，会去循环读取pool->d.next链表上的缓存池，
            // 并且比较剩余空间大小,是否可以容乃新的内存块存储）
            // nelts 已用元素数量不变
        }
    }

    /* 最新的元素指针 地址 */
    elt = (u_char *)a->elts + a->size * a->nelts;
    a->nelts++; //只分配一个元素，所以元素数量+1

    return elt;
}

void *
ngx_array_push_n(ngx_array_t *a, ngx_uint_t n)
{
    void *elt, *new;
    size_t size;
    ngx_uint_t nalloc;
    ngx_pool_t *p;

    size = n * a->size;

    if (a->nelts + n > a->nalloc)
    {

        /* the array is full */

        p = a->pool;

        if ((u_char *)a->elts + a->size * a->nalloc == p->d.last && p->d.last + size <= p->d.end)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.last += size;
            a->nalloc += n;
        }
        else
        {
            /* allocate a new array */

            nalloc = 2 * ((n >= a->nalloc) ? n : a->nalloc);

            new = ngx_palloc(p, nalloc * a->size);
            if (new == NULL)
            {
                return NULL;
            }

            ngx_memcpy(new, a->elts, a->nelts * a->size);
            a->elts = new;
            a->nalloc = nalloc;
        }
    }

    elt = (u_char *)a->elts + a->size * a->nelts;
    a->nelts += n;

    return elt;
}
