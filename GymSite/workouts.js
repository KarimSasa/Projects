'use strict'

const root = document.documentElement;

document.querySelectorAll('.accordion-label').forEach(button => {
    button.addEventListener('click', buttonClick);
});

function buttonClick(event) {
    const btn = event.target;
    btn.classList.toggle('open');
    const content = btn.nextElementSibling;
    content.classList.toggle('open');

    document.querySelectorAll('.accordion-label').forEach(otherBtn => {
        if (otherBtn !== btn && otherBtn.classList.contains('open')) {
            otherBtn.classList.remove('open');
            otherBtn.nextElementSibling.classList.remove('open');
        }
    });

    root.style.setProperty('--content-height', `${content.scrollHeight}px`);
}