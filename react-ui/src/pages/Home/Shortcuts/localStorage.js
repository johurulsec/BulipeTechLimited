export const getShortcuts = () => {
	console.log("getShortcuts() of localStorage.js");

	const data = localStorage.getItem("shortcuts");
	return data ? JSON.parse(data) : [];
};


// Extract domain from any user-entered URL
const getDomain = (rawUrl) => {
	console.log("getDomain() of localStorage.js");

	try {
		const urlObj = new URL(rawUrl.startsWith("http") ? rawUrl : `https://${rawUrl}`);
		return urlObj.hostname;
	// eslint-disable-next-line no-unused-vars
	} catch (e) {
		console.error("Invalid URL:", rawUrl);
		return null;
	}
};

export const saveShortcut = (shortcut) => {
	console.log("saveShortcut() of localStorage.js");

	const { name, url } = shortcut;
	const domain = getDomain(url);
	if (!domain) return;

	const favicon = `https://icons.duckduckgo.com/ip3/${domain}.ico`;

	const enhancedShortcut = {
		name,
		url: url.startsWith("http") ? url : `https://${url}`,
		favicon,
	};

	const current = getShortcuts();
	current.push(enhancedShortcut);
	localStorage.setItem("shortcuts", JSON.stringify(current));
};
