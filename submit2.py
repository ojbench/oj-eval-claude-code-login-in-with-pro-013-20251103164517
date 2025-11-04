#!/usr/bin/env python3
import os
import requests
import json

token = os.environ.get('ACMOJ_TOKEN')
if not token:
    print("Error: ACMOJ_TOKEN not set")
    exit(1)

api_base = "https://acm.sjtu.edu.cn/OnlineJudge/api/v1"
headers = {
    "Authorization": f"Bearer {token}",
    "Content-Type": "application/x-www-form-urlencoded",
    "User-Agent": "ACMOJ-Python-Client/2.2"
}

git_url = "https://github.com/ojbench/oj-eval-claude-code-login-in-with-pro-013-20251103164517.git"

data = {"language": "git", "code": git_url}

try:
    response = requests.post(f"{api_base}/problem/2671/submit", headers=headers, data=data, timeout=10)
    print(f"Status Code: {response.status_code}")
    print(f"Response Text: {response.text}")
    if response.status_code == 200 or response.status_code == 201:
        result = response.json()
        print(f"Result: {json.dumps(result, indent=2)}")
        if 'id' in result:
            print(f"\nSubmission ID: {result['id']}")
            with open('/workspace/submission_ids.log', 'a') as f:
                f.write(f"{result['id']}\n")
except Exception as e:
    print(f"Error: {e}")
