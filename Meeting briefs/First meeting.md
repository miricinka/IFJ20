# First *real* meeting

### Questions:
* Did everyone finish their tasks?
	* Everyone has still something to finish
* How will we proceed?
	* Martin and Mirka will start implementing the scanner
	* Tomáš and Aleš will look into precedence analysis
* Where to start?
	* Scanner and precedence, then we will continue by splitting parser into minor tasks
* Set Github commit standards?
	* Was not debated as Mirka had not looked into advanced Git and Githubing

## Personal insights 
### Martin - Automated testing
* **When should we test?**
	* From the start (maybe TDD?), would help a lot to debug code 
	* Later, would at least tell us, that the compiler behaves as it should
* **How should we test?**
	* Black-Box, quick but not that reliable, minimum
	* White-Box, slower, better test coverage
	* Both, slow, but we would know exactly how the compiler behaves

### Miru - How to pass tokens from scanner to parser
* Same as previous year, type value and modifies

