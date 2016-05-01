/* 
* Skeleton V1.0.3
* Copyright 2011, Dave Gamache
* www.getskeleton.com
* Free to use under the MIT license.
* http://www.opensource.org/licenses/mit-license.php
* 7/17/2011
*/	

jQuery(document).ready(function($) {

	
		/* Superfish
	================================================== */
	$(function(){ // run after page loads
		$('#header ul.menu, #navigation ul.menu')
		.find('li.current_page_item,li.current_page_parent,li.current_page_ancestor,li.current-cat,li.current-cat-parent,li.current-menu-item')
			.addClass('active')
			.end()
			.superfish({autoArrows	: true});
			
		jQuery('#navigation select').change(function() {
			window.location = jQuery(this).find("option:selected").val();
		});
	});
	
	// Forum Login
	
	$(function(){ // run after page loads
		$("#toggle").click(function() { 
	    // hides matched elements if shown, shows if hidden
	    $("#login-form").slideToggle();
	  });
	});

	// Style Tags
	
	$(function(){ // run after page loads
		$('p.tags a')
		.wrap('<span class="st_tag" />');
	});
	

	// Toggle Slides
	
	$(function(){ // run after page loads
			$(".toggle_container").hide(); 
			//Switch the "Open" and "Close" state per click then slide up/down (depending on open/close state)
			$("p.trigger").click(function(){
				$(this).toggleClass("active").next().slideToggle("normal");
				return false; //Prevent the browser jump to the link anchor
			});
	});
	
	// valid XHTML method of target_blank
	$(function(){ // run after page loads
		$('a[rel*=external]').click( function() {
			window.open(this.href);
			return false;
		});
	});


	/* Tabs Activiation
	================================================== */
	var tabs = $('ul.tabs');
	tabs.each(function(i) {
		//Get all tabs
		var tab = $(this).find('> li > a');
		$("ul.tabs li:first").addClass("active").fadeIn('fast'); //Activate first tab
		$("ul.tabs li:first a").addClass("active").fadeIn('fast'); //Activate first tab
		$("ul.tabs-content li:first").addClass("active").fadeIn('fast'); //Activate first tab
		
		tab.click(function(e) {
			
			//Get Location of tab's content
			var contentLocation = $(this).attr('href') + "Tab";
			
			//Let go if not a hashed one
			if(contentLocation.charAt(0)=="#") {
			
				e.preventDefault();
			
				//Make Tab Active
				tab.removeClass('active');
				$(this).addClass('active');
				
				//Show Tab Content & add active class
				$(contentLocation).show().addClass('active').siblings().hide().removeClass('active');
				
			} 
		});
	});


	/* Resizing video players on small screens
	================================================== */
	$(function() {
		var iframes = document.getElementsByTagName('iframe');
		
		for (var i = 0; i < iframes.length; i++) {
			var iframe = iframes[i];
			var players = /www.youtube.com|player.vimeo.com|youtube-nocookie.com|kickstarter.com/;
			if(iframe.src.search(players) !== -1) {
				var videoRatio = (iframe.height / iframe.width) * 100;
			
				iframe.style.position = 'absolute';
				iframe.style.top = '0';
				iframe.style.left = '0';
				iframe.width = '100%';
				iframe.height = '100%';
				
				var div = document.createElement('div');
				div.className = 'video-wrap';
				div.style.width = '100%';
				div.style.position = 'relative';
				div.style.paddingTop = videoRatio + '%';
				
				var parentNode = iframe.parentNode;
				parentNode.insertBefore(div, iframe);
				div.appendChild(iframe);
			}
		}
	});
});