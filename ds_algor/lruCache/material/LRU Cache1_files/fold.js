var fold_height = {}
function $use(contentid,buttonid)
{
	var d=document.getElementById(contentid);
	var sb=document.getElementById(buttonid);
	if(d.style.display=='none'){
		// Unfold
		//var h=d.offsetHeight;
		//var maxh=300;
		var h=0;
		var H=fold_height[contentid];
		function unfold(){
			h+=50; //设置层展开的速度
			if(h>=H){
				d.style.height=H+'px';
				clearInterval(iIntervalId);
			}else{
				d.style.display='block';
				d.style.height=h+'px';
			}
		}
		iIntervalId=setInterval(unfold,2);
		sb.innerHTML='[-]';
	}
	else{
		var h=d.offsetHeight;
		fold_height[contentid] = h;
		function fold(){
			h-=50;//设置层收缩的速度
			if(h<=0){
				d.style.display='none';
				clearInterval(iIntervalId);
			}
			else{
				d.style.height=h+'px';
			}
		}
	iIntervalId=setInterval(fold,2);
	sb.innerHTML='[+]';
	}
}