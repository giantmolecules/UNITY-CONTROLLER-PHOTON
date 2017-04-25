using UnityEngine;
using System.Collections;
using UnityEngine.Experimental.Networking;

public class CallPhotonFunction: MonoBehaviour {

	public string deviceID;
	public string accessToken;
	private string requestURL;
	private string func;
	private Renderer theRenderer;
	private float timeNow;
	private float timeThen;
	private float interval;
	private bool okToSend = false;

	void Start() {
		//Debug.Log ("START");
		func = "ledSwitch";
		requestURL = "https://api.spark.io/v1/devices/" + deviceID + "/" + func + "/";
		theRenderer = GetComponent<Renderer> ();
		theRenderer.material.color = new Color (1f, 1f, 1f);
		interval = 2.0f;
		timeThen = Time.unscaledTime;
	}

	void OnTriggerEnter(Collider other){
		Debug.Log ("ENTER");
		if (okToSend) {
			Debug.Log ("starting enter coroutine...");
			StartCoroutine (Upload (1));
			timeThen = Time.unscaledTime;
			okToSend = false;
		}
		theRenderer.material.color = new Color (0f, 0f, 1f);
	}

	void OnTriggerExit(Collider other){
		Debug.Log ("EXIT");
		if (okToSend) {
			Debug.Log ("starting exit coroutine...");
			StartCoroutine (Upload (0));
			timeThen = Time.unscaledTime;
			okToSend = false;
		}
		theRenderer.material.color = new Color (1f, 1f, 1f);
	}

	IEnumerator Upload(int value) {
		WWWForm form = new WWWForm();
		form.AddField("args=", value.ToString());
		form.AddField("access_token", accessToken);
		UnityEngine.Networking.UnityWebRequest www = UnityEngine.Networking.UnityWebRequest.Post(requestURL, form);
		yield return www.Send();
		Debug.Log ("SENDING...");
		if(www.isError) {
			Debug.Log ("ERROR:");
			Debug.Log(www.error);
		}
		else {
			Debug.Log("Form upload complete!");
		}
	}

	void Update(){
		timeNow = Time.unscaledTime;
		if (timeNow - timeThen >= interval) {
			okToSend = true;
		}
	}
}