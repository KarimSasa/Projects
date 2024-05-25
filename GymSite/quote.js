document.addEventListener('DOMContentLoaded', function() {
    const quoteElement = document.getElementById('quote');
    const navLinks = document.querySelectorAll('nav ul li a');
    let quotes = []; 
    let currentIndex = parseInt(sessionStorage.getItem('currentQuoteIndex'), 10) || 0; 
    
    // Main function to display the quotes
    function displayQuote() {
        if (quotes.length > 0 && quotes[currentIndex]) {
            // Fade in the new quote for the animation
            quoteElement.classList.remove('hidden');
            const quote = quotes[currentIndex];
            quoteElement.textContent = `"${quote.q}" — ${quote.a}`;
            currentIndex = (currentIndex + 1) % quotes.length;
        } else {
            quoteElement.textContent = "No quotes loaded.";
        }
    }
    
    // Fetch the quotes from the API, and have error handling
    function fetchQuotes() {
        fetch('https://api.allorigins.win/raw?url=https://zenquotes.io/api/quotes')
            .then(response => response.json())
            .then(data => {
                quotes = data; 
                displayQuote(); 
            })
            .catch(error => {
                quoteElement.textContent = "Failed to load inspirational quotes.";
                console.error('Error fetching quotes:', error);
            });
    }
    
    // Load the quotes upon website loading
    fetchQuotes();
    
    // Quote Fade out/in animation
    setInterval(() => {
        quoteElement.classList.add('hidden');
        setTimeout(displayQuote, 3000); 
    }, 10000); 

    // Function to change to the next quote in the array if user goes to another section
    navLinks.forEach(link => {
        link.addEventListener('click', (e) => {
            e.preventDefault(); 
            currentIndex = (currentIndex + 1) % quotes.length; 
            sessionStorage.setItem('currentQuoteIndex', currentIndex); 
            displayQuote();
            window.location.href = link.href; 
        });
    });
});
