'use strict';

const leftBtn = document.querySelector('.left');
const rightBtn = document.querySelector('.right');

const carouselItems =  Array.from(document.querySelectorAll('.carousel-item'));
const navItems =  Array.from(document.querySelectorAll('.nav-item'));
const CAROUSEL_SIZE = carouselItems.length;

leftBtn.addEventListener('click', swipe);
rightBtn.addEventListener('click', swipe);

function swipe(e){
    const currentCarouselItem = document.querySelector('.carousel-item.active');
    const currentIndex = carouselItems.indexOf(currentCarouselItem);

    let nextIndex;

    if(e.currentTarget.classList.contains('left')){
        if (currentIndex === 0){
            nextIndex = CAROUSEL_SIZE - 1;
        }
        else {
            nextIndex = currentIndex - 1;
        }
    }
    else {
        if (currentIndex === CAROUSEL_SIZE - 1){
            nextIndex = 0;
        }
        else {
            nextIndex = currentIndex + 1;
        }

    }



    carouselItems[nextIndex].classList.add('active');
    navItems[nextIndex].classList.add('active');
    currentCarouselItem.classList.remove('active');
    navItems[currentIndex].classList.remove('active');


}

const carouselNav = document.querySelector('.carousel-nav');
carouselNav.addEventListener('click', function(e) {
    if (e.target.classList.contains('nav-item')) {
        const targetIndex = navItems.indexOf(e.target);
        const currentIndex = carouselItems.findIndex(item => item.classList.contains('active'));

        if (targetIndex !== currentIndex) {
            carouselItems[targetIndex].classList.add('active');
            navItems[targetIndex].classList.add('active');
            carouselItems[currentIndex].classList.remove('active');
            navItems[currentIndex].classList.remove('active');
        }
    }


});

