document.addEventListener('DOMContentLoaded', function() {
    fetch('nutrition.json')  
        .then(response => response.json())
        .then(data => initializeTabs(data.plans));
    
    // Intitialization
    function initializeTabs(plans) {
        const tabsContainer = document.querySelector('#diet-tabs .tab');
        const contentContainer = document.querySelector('#nutrition-content');
        const dietChartImage = document.querySelector('#diet-chart img');
        const mealPlanTable = document.querySelector('#workout-schedule table');

        plans.forEach((plan, index) => {
            const button = document.createElement('button');
            button.className = 'tablinks';
            button.textContent = plan.name;
            button.addEventListener('click', function(event) { openTab(event, plan, index); });
            tabsContainer.appendChild(button);

            const content = document.createElement('div');
            content.id = `Tab${index + 1}`;
            content.className = 'tabcontent';
            content.innerHTML = `
                <h3>Recommended Carbohydrates</h3>
                <ul>${plan.carbohydrates.map(carb => `<li>${carb}</li>`).join('')}</ul>
                <h3>Recommended Proteins</h3>
                <ul>${plan.proteins.map(prot => `<li>${prot}</li>`).join('')}</ul>
                <h3>Recommended Fats</h3>
                <ul>${plan.fats.map(fat => `<li>${fat}</li>`).join('')}</ul>
            `;
            contentContainer.appendChild(content);
        });
        
        // Button Animations
        tabsContainer.querySelectorAll('.tablinks').forEach(button => {
            button.addEventListener('click', (event) => {
                tabsContainer.querySelectorAll('.tablinks').forEach(btn => btn.classList.remove('active'));

                event.currentTarget.classList.add('active');
            });
        });

        // Button Animations
        document.addEventListener('click', (event) => {
            if (!event.target.matches('.tablinks')) {
                tabsContainer.querySelectorAll('.tablinks').forEach(button => {
                    button.classList.remove('active');
                    button.style.backgroundColor = ""; 
                });
            }
        });
        

        if (tabsContainer.children.length > 0) {
            tabsContainer.children[0].click();
        }
    }
    
    // Tab Functionality
    function openTab(evt, plan, index) {
        let tabcontent = document.getElementsByClassName("tabcontent");
        for (let i = 0; i < tabcontent.length; i++) {
            tabcontent[i].style.display = "none";
        }

        let tablinks = document.getElementsByClassName("tablinks");
        for (let i = 0; i < tablinks.length; i++) {
            tablinks[i].style.backgroundColor = "";
        }

        document.getElementById(`Tab${index + 1}`).style.display = "block";

        document.querySelector('#diet-chart img').src = plan.pieChartUrl;
        document.querySelector('#diet-chart img').alt = `Dietary Values for ${plan.name}`;

        updateMealPlanTable(plan.mealPlan);

        evt.currentTarget.style.backgroundColor = "var(--button-background-color)";
    }
    
    // Meal Plan Table Functionality
    function updateMealPlanTable(mealPlan) {
        const table = document.querySelector('#mealPlanTable');
        if (!table) {
            console.error('Table element not found!');
            return;
        }
    
        let tableHtml = `
            <tr>
                <th>Day</th>
                <th>Breakfast</th>
                <th>Lunch</th>
                <th>Dinner</th>
            </tr>`;
    
        mealPlan.forEach(dayPlan => {
            tableHtml += `
                <tr>
                    <td>${dayPlan.day}</td>
                    <td>${dayPlan.breakfast}</td>
                    <td>${dayPlan.lunch}</td>
                    <td>${dayPlan.dinner}</td>
                </tr>`;
        });
    
        table.innerHTML = tableHtml;
    }
});
