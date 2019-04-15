/*
=================================================================================================================================================
---------------------------------
VARIABLES USED
--------------------------------
1. key_pressed - javascript object for the keypressed by the user. used to check if enter is used to make the search 
2. site_url - Url of the site
3. check_btn - stores booleen value of the state of the (exact match) checkbox 
4. start_time- The time in history from which the search is made.
5. count- The count of the number of number times the url (site_url ) was visted.
6. duration - The duration in count is to be found.
7. matched_sites - A array of the VisitItems returned by the chrome.history.getVisits() function.
8. visted_site- A varaible to store the ith element in the matched_sites array while iterating.

----------------------------
FUNCTIONS USED
----------------------------
1. track - Complete logic to find the count.
2. chrome.history.getVisits(object details, function callback)- Retrieves information about visits to a URL.
3. chrome.history.search(object query, function callback) -Searches the history for the last visit time of each page matching the query.

=================================================================================================================================================
*/

$("#submit").click(function () {
    track();
});

$(document).keypress(function(key_pressed){
    if(key_pressed.which===13) //compares the which code of the key pressed with that of Enter key
        track();
});

function track() {

    var duration = Number($("#duration").val()); 
    
    var site_url = $("#page_url").val(); 
    var check_btn = $('input[type="checkbox"]');

    /*
    (new Date).getTime() get the current system time
    the Duration enter is converted into microsec
    and the start time is set to the current time- the duration to search
    */ 
    var start_time = (new Date).getTime() - (1000 * 60 * 60 * duration);  
    var count = 0;

    if (site_url == '') // check for empty search
        alert("Please enter url First!!!");

    else {
        /* checking  if the url contained the https:// starting or not
            for searches like (www.google.com) as the exact URL should be (https://www.google.com)  
        if it is absent then it is addded.
        */
        if (site_url.slice(0, 7) != 'http://' && site_url.slice(0, 8) != 'https://')
            site_url = "https://" + site_url;
        

        // checking if the Exact match Checkbox is Checked or not.
        if (check_btn.prop("checked") == true) {
            /*
            For the Exact match case- Chrome api function -chrome.history.getVisits function is used. 
            it checks for the URL in the browser's history and returns a array of matched Visits made to the URL.
            then it is checked if a particular visit is made in the our required duration if yes then that visit is counted. 
            */
            chrome.history.getVisits({ "url": site_url }, function (matched_sites) {
                if (matched_sites == [])
                    count = 0;
                else {
                    // the matched_sites array is traversed 
                    matched_sites.forEach(function (visted_site) {
                        // the time of visit of a particular Vist is checked if it lies within given duration or not. 
                        if (visted_site.visitTime - start_time >= 0)
                            count++;
                    });
                }
                // the Extension popup html is updated to show the result
                $("#output").html("<h3>Number of Vists= " + count + "</h3>");
            });

        }
        else if (check_btn.prop("checked") == false) {
            /*
            For the Domain match case- the chrome.history.search function is used to get Visit whose text(A free-text query to the history service ) contain the site_url
            this function returns  the last visit time of each page matching the query. The url of the returned visits is compared to the Site_url to check if it is the site or just the url is mentioned in another site 
            and for each of the filtered vists the Exact match search is made  */

            $("#output").html("<h3>Number of Vists= " + count + "</h3>");

            chrome.history.search({
                'text': site_url, //text to search
                'startTime': start_time, // start time in history
                'maxResults': 1000000 // max results by default set to 100 so, was set to a very large.  
            }, function (matched_sites) {
                // Iterating through the returned list of sites.
                matched_sites.forEach(visted_site => {
                    // check if the site is what we searched or just a mention on another site.
                    if (visted_site.url == site_url || site_url == (visted_site.url).slice(0, site_url.length)) {

                        // Exact Match search for the filtered sites.
                        chrome.history.getVisits({ "url": visted_site.url }, function (matched_sites) {
                            if (matched_sites == [])
                                count = 0;
                            else {
                                matched_sites.forEach(function (visted_site) {
                                    if (visted_site.visitTime - start_time >= 0)
                                        count++;
                                });
                            }
                           // the Extension popup html is updated to show the result
                            $("#output").html("<h3>Number of Vists= " + count + "</h3>");
                        });
                    }
                });
            });
        }
    }
}