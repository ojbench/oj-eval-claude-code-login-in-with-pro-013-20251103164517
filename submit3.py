#!/usr/bin/env python3
import os
import requests
import json

token = os.environ.get('ACMOJ_TOKEN')
if not token:
    print("Error: ACMOJ_TOKEN not set")
    exit(1)

# Read the code file
with open('src/map.hpp', 'r') as f:
    code_content = f.read()

api_base = "https://acm.sjtu.edu.cn/OnlineJudge/api/v1"
headers = {
    "Authorization": f"Bearer {token}",
    "Content-Type": "application/x-www-form-urlencoded",
    "User-Agent": "ACMOJ-Python-Client/2.2"
}

data = {"language": "cpp", "code": code_content}

print("Submitting to problem 2671...")
try:
    response = requests.post(f"{api_base}/problem/2671/submit", headers=headers, data=data, timeout=30)
    print(f"Status Code: {response.status_code}")
    print(f"Response Text: {response.text}")
    if response.status_code == 200 or response.status_code == 201:
        result = response.json()
        print(f"Result: {json.dumps(result, indent=2)}")
        if 'id' in result:
            submission_id = result['id']
            print(f"\nSubmission ID for problem 2671: {submission_id}")
            with open('/workspace/submission_ids.log', 'a') as f:
                f.write(f"2671: {submission_id}\n")
except Exception as e:
    print(f"Error: {e}")
